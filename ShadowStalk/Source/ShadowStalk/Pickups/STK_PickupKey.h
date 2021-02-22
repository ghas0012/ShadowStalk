// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STK_PickupBase.h"
#include "STK_PickupKey.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWSTALK_API ASTK_PickupKey : public ASTK_PickupBase
{
	GENERATED_BODY()
	
public:
	virtual EPickupType GetPickupType() override;

};
