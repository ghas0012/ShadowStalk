// Fill out your copyright notice in the Description page of Project Settings.


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

    CurrentSpeed = WalkSpeed;

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
                FCollisionShape::MakeCapsule(CapsuleComp->GetScaledCapsuleRadius() * 0.5f, CapsuleComp->GetScaledCapsuleHalfHeight() + 0.5f),
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
        VelocityVector.Y = FMath::Lerp(VelocityVector.X, 0.0f, DeltaTime * FrictionLerp);
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

    //TODO - Temp Code, will update once we have actual sizes
    if (bIsCrawling)
    {
        CapsuleComp->SetCapsuleHalfHeight(35.0f);
    }
    else
    {
        CapsuleComp->SetCapsuleHalfHeight(70.0f);
    }

    UpdateComponentVelocity();

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
        return;

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

    CurrentSpeed = SprintSpeed;
    bIsCrawling = false;
}

void USTK_EntityMovementComponent::Walk()
{
    if (bInputLocked)
    {
        return;
    }

    CurrentSpeed = WalkSpeed;
    bIsCrawling = false;
}

void USTK_EntityMovementComponent::Crawl()
{
    if (bInputLocked)
        return;

    CurrentSpeed = CrawlSpeed;
    bIsCrawling = true;

}
