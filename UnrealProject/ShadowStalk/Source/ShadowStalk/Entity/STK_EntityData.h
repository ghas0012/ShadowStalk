// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Hamidreza Ghasemi
  Date Modified: 3/23/2021
  Comment/Description:

     A blueprint-friendly struct to hold entity's movement data.

  ChangeLog:
  H 3/12/2021: Init.
*/

#pragma once

#include "STK_EntityData.generated.h"

USTRUCT(BlueprintType)
struct FSTK_EntityData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementData")
    float m_MouseLook_X;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementData")
    float m_MouseLook_Y;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementData")
    float m_MouseLook_VerticalLookLimitAngle = 85.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementData")
    float m_JumpStrength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementData")
    float m_FrictionLerp = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementData")
    float m_Acceleration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementData")
    float m_WalkSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementData")
    float m_SprintSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementData")
    float m_CrawlSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementData")
    float m_AirControl;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementData")
    float m_CapsuleRadius = 50.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementData")
    float m_CapsuleHalfHeight = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementData")
    float m_CrawlCapsuleHalfHeight = 50.f;
};