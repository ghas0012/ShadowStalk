// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Christian Young, Hamidreza Ghasemi

// Changelog:
// - Class init.

#include "STK_EntityMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"



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

    GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Emerald, FString::Printf(TEXT("Grounded: %d"), bIsGrounded));

    FVector InputAcceleration;

    float AirControlMultiplier = bIsGrounded ? 1 : AirControl;

    InputAcceleration = ConsumeInputVector() * Acceleration * AirControlMultiplier;

    GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow, FString::Printf(TEXT("Vector: %f %f %f"), InputAcceleration.X, InputAcceleration.Y, InputAcceleration.Z));

    VelocityVector += InputAcceleration * DeltaTime;
    VelocityVector = VelocityVector.GetClampedToMaxSize2D(CurrentSpeed);

    GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Red, FString::Printf(TEXT("CurrentSpeed: %f"), CurrentSpeed));

    if ((bIsGrounded) && InputAcceleration.SizeSquared() < 0.5f)
    {
        VelocityVector.X = FMath::Lerp(VelocityVector.X, 0.0f, DeltaTime * FrictionLerp);
        VelocityVector.Y = FMath::Lerp(VelocityVector.Y, 0.0f, DeltaTime * FrictionLerp);
    }

    FVector DesiredMovementThisFrame = VelocityVector * DeltaTime;
    
    GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Purple, FString::Printf(TEXT("VelocityVector: %f %f %f"), VelocityVector.X, VelocityVector.Y, VelocityVector.Z));

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

void USTK_EntityMovementComponent::HandleCrawlTransition(float DeltaTime)
{
    CrawlTransitionPercentage += DeltaTime * CrawlTransitionSpeed;

    if (CrawlTransitionPercentage >= 1.0f)
        CrawlTransitionPercentage = 1.0f;

    if (bIsCrawling)
    {
        CapsuleComp->SetCapsuleHalfHeight(FMath::Lerp(CrawlTransitionInitHalfHeight, CapsuleCrawlHalfHeight, CrawlTransitionPercentage));
    }
    else
    {
        CapsuleComp->SetCapsuleHalfHeight(FMath::Lerp(CrawlTransitionInitHalfHeight, CapsuleStandingHalfHeight, CrawlTransitionPercentage));
    }
}

void USTK_EntityMovementComponent::Reset()
{
    bIsGrounded = false;
    VelocityVector = FVector::ZeroVector;
    CapsuleComp->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

    VelocityAtJump = FVector::ZeroVector;
    LockInput(false);
}

void USTK_EntityMovementComponent::Jump(float jumps) //TODO - FUCKING FIX THIS DUDE HOLY FUCKING SHIT
{
    if (bInputLocked)
    {
        return;
    }

    JumpStrength = jumps;

    VelocityAtJump = VelocityVector;

    CapsuleComp->AddImpulse(FVector::UpVector * jumps);
    bIsGrounded = false;
}

void USTK_EntityMovementComponent::LockInput(bool b)
{
    bInputLocked = b;
    VelocityAtJump = FVector::ZeroVector;
    CapsuleComp->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
}

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

float USTK_EntityMovementComponent::GetForwardVelocity()
{
    return FMath::Abs(FVector::DotProduct(CapsuleComp->GetForwardVector(), VelocityVector));
}
