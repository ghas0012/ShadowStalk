// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_EntityMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"


/// <summary>
/// This tick function calculates the desired movement based on the input acceleration.
/// It slides the collider along surfaces so there's no intersection.
/// It also checks if the collider is on the ground and calls a method to handle Crawling height.
/// </summary>
void USTK_EntityMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    if (bInputLocked)
    {
        return;
    }

    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
    {
        return;
    }

    bIsGrounded = false;

    FCollisionQueryParams CollisionParameters;
    CollisionParameters.AddIgnoredComponent(CapsuleComp);

    FHitResult GroundCheck;

    FVector BottomOfCollider = CapsuleComp->GetRelativeLocation() - FVector::UpVector * CapsuleComp->GetScaledCapsuleHalfHeight();

    if
    (
        GetWorld()->SweepSingleByChannel
        (
            GroundCheck,
            CapsuleComp->GetRelativeLocation(),
            BottomOfCollider,
            FQuat::Identity,
            ECollisionChannel::ECC_GameTraceChannel1,
            FCollisionShape::MakeCapsule(CapsuleComp->GetScaledCapsuleRadius() * 0.5f, CapsuleComp->GetScaledCapsuleHalfHeight()-50),
            CollisionParameters
        )
    )
    {
        if (GroundCheck.bBlockingHit)
        {
            bIsGrounded = true;
            VelocityVector.Z = 0;
        }
    }

    //GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Emerald, FString::Printf(TEXT("Grounded: %d"), bIsGrounded));

    FVector InputAcceleration;

    float AirControlMultiplier = bIsGrounded ? 1 : AirControl;

    InputAcceleration = ConsumeInputVector() * Acceleration * AirControlMultiplier;

    //GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow, FString::Printf(TEXT("Vector: %f %f %f"), InputAcceleration.X, InputAcceleration.Y, InputAcceleration.Z));

    VelocityVector += InputAcceleration * DeltaTime;
    VelocityVector = VelocityVector.GetClampedToMaxSize2D(CurrentSpeed);

    //GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Red, FString::Printf(TEXT("CurrentSpeed: %f"), CurrentSpeed));

    if ((bIsGrounded) && InputAcceleration.SizeSquared() < 0.5f)
    {
        VelocityVector.X = FMath::Lerp(VelocityVector.X, 0.0f, DeltaTime * FrictionLerp);
        VelocityVector.Y = FMath::Lerp(VelocityVector.Y, 0.0f, DeltaTime * FrictionLerp);
    }

    FVector DesiredMovementThisFrame = VelocityVector * DeltaTime;
    
    //GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Purple, FString::Printf(TEXT("VelocityVector: %f %f %f"), VelocityVector.X, VelocityVector.Y, VelocityVector.Z));

    if (!DesiredMovementThisFrame.IsNearlyZero())
    {
        FHitResult Hitres;

        SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hitres);

        if (Hitres.IsValidBlockingHit())
        {
            SlideAlongSurface(DesiredMovementThisFrame, 1.0f - Hitres.Time, Hitres.Normal, Hitres);
        }
    }

    if (CrawlTransitionPercentage != 1.0f)
    {
        HandleCrawlTransition(DeltaTime);
    }

    UpdateComponentVelocity();

}


/// <summary>
/// Smoothly transition the height of the collider to match whether or not the entity is crawling.
/// </summary>
void USTK_EntityMovementComponent::HandleCrawlTransition(float DeltaTime)
{
    CrawlTransitionPercentage += DeltaTime * CrawlTransitionSpeed;

    if (CrawlTransitionPercentage >= 1.0f)
        CrawlTransitionPercentage = 1.0f;

    if (bIsCrawling)
    {
        float finalHalfHeight = FMath::Lerp(CrawlTransitionInitHalfHeight, CapsuleCrawlHalfHeight, CrawlTransitionPercentage);
        float diff = CapsuleComp->GetScaledCapsuleHalfHeight() - finalHalfHeight;
        CapsuleComp->SetCapsuleHalfHeight(finalHalfHeight);
        CapsuleComp->SetRelativeLocation(CapsuleComp->GetRelativeLocation() - FVector(0, 0, diff));
    }
    else
    {
        float finalHalfHeight = FMath::Lerp(CrawlTransitionInitHalfHeight, CapsuleStandingHalfHeight, CrawlTransitionPercentage);
        float diff = CapsuleComp->GetScaledCapsuleHalfHeight() - finalHalfHeight;
        CapsuleComp->SetCapsuleHalfHeight(finalHalfHeight);
        CapsuleComp->SetRelativeLocation(CapsuleComp->GetRelativeLocation() - FVector(0, 0, diff));
    }
}


/// <summary>
/// Stops all movement of the collider.
/// </summary>
void USTK_EntityMovementComponent::Reset()
{
    bIsGrounded = false;
    VelocityVector = FVector::ZeroVector;
    CapsuleComp->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

    VelocityAtJump = FVector::ZeroVector;
    LockInput(false);
}


/// <summary>
/// Adds an impulse for jumping.
/// </summary>
void USTK_EntityMovementComponent::Jump(float jumpStrength) //TODO - FIX THIS DUDE HOLY CRAP
{
    if (bInputLocked)
    {
        return;
    }

    JumpStrength = jumpStrength;

    VelocityAtJump = VelocityVector;

    CapsuleComp->AddImpulse(FVector::UpVector * jumpStrength);

    bIsGrounded = false;
}


void USTK_EntityMovementComponent::LockInput(bool b)
{
    bInputLocked = b;
    VelocityAtJump = FVector::ZeroVector;
    CapsuleComp->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
}


/// <summary>
/// Updates the movement speed of the component. Disables crawl.
/// </summary>
void USTK_EntityMovementComponent::Sprint()
{
    if (bInputLocked)
    {
        return;
    }

    CrawlTransitionInitHalfHeight = CapsuleComp->GetScaledCapsuleHalfHeight();
    CrawlTransitionPercentage = 0;
    CurrentSpeed = SprintSpeed;
    bIsCrawling = false;
}


/// <summary>
/// Updates the movement speed of the component. Disables crawl.
/// </summary>
void USTK_EntityMovementComponent::Walk()
{

    if (bInputLocked)
    {
        return;
    }

    CrawlTransitionInitHalfHeight = CapsuleComp->GetScaledCapsuleHalfHeight();
    CrawlTransitionPercentage = 0;
    CurrentSpeed = WalkSpeed;
    bIsCrawling = false;

}


/// <summary>
/// Updates the movement speed of the component. Enables crawl.
/// </summary>
void USTK_EntityMovementComponent::Crawl()
{

    if (bInputLocked)
    {
        return;
    }

    CrawlTransitionInitHalfHeight = CapsuleComp->GetScaledCapsuleHalfHeight();
    CrawlTransitionPercentage = 0;
    CurrentSpeed = CrawlSpeed;
    bIsCrawling = true;

}


bool USTK_EntityMovementComponent::GetIsGrounded()
{
    return bIsGrounded;
}


/// <summary>
/// A Ufunction for the anim blueprint to apply run animation.
/// </summary>
float USTK_EntityMovementComponent::GetForwardVelocity()
{
    return FMath::Abs(FVector::DotProduct(CapsuleComp->GetForwardVector(), VelocityVector));
}

void USTK_EntityMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(USTK_EntityMovementComponent, CapsuleComp);
    DOREPLIFETIME(USTK_EntityMovementComponent, CurrentSpeed);

}
