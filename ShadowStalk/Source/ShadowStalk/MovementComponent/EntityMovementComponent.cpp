// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityMovementComponent.h"
#include "Components/CapsuleComponent.h"



void UEntityMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

    VelocityVector += InputAcceleration * DeltaTime;
    VelocityVector = VelocityVector.GetClampedToMaxSize2D(CurrentSpeed);
    
    if ((bIsGrounded) && InputAcceleration.SizeSquared() < 0.5f)
    {
        VelocityVector.X = FMath::Lerp(VelocityVector.X, 0.0f, DeltaTime * FrictionLerp);
        VelocityVector.Y = FMath::Lerp(VelocityVector.X, 0.0f, DeltaTime * FrictionLerp);
    }

    FVector DesiredMovementThisFrame = VelocityVector * DeltaTime;

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

void UEntityMovementComponent::Reset()
{
    bIsGrounded = false;
    VelocityVector = FVector::ZeroVector;
    CapsuleComp->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

    VelocityAtJump = FVector::ZeroVector;
    LockInput(false);
}

void UEntityMovementComponent::Jump(float jumps) //TODO - FUCKING FIX THIS DUDE HOLY FUCKING SHIT
{
    if (bInputLocked)
        return;

    VelocityAtJump = VelocityVector;

    CapsuleComp->AddImpulse(FVector::UpVector * JumpStrength);
    bIsGrounded = false;
}

void UEntityMovementComponent::LockInput(bool b)
{
    bInputLocked = b;
    VelocityAtJump = FVector::ZeroVector;
    CapsuleComp->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
}

void UEntityMovementComponent::Sprint()
{
    if (bInputLocked)
    {
        return;
    }

    CurrentSpeed = SprintSpeed;
    bIsCrawling = false;
}

void UEntityMovementComponent::Walk()
{
    if (bInputLocked)
    {
        return;
    }

    CurrentSpeed = WalkSpeed;
    bIsCrawling = false;
}

void UEntityMovementComponent::Crawl()
{
    if (bInputLocked)
        return;

    CurrentSpeed = CrawlSpeed;
    bIsCrawling = true;

}
