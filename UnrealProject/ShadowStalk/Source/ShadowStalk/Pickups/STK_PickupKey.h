// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Hamidreza Ghasemi
  Date Modified: 3/12/2021
  Comment/Description: *Description of Class*
  ChangeLog:
  H 3/12/2021: *Description of Change*
*/

#pragma once

#include "CoreMinimal.h"
#include "STK_PickupBase.h"
#include "STK_PickupKey.generated.h"

UCLASS()
class SHADOWSTALK_API ASTK_PickupKey : public ASTK_PickupBase
{
	GENERATED_BODY()
	
public:
	virtual EPickupType GetPickupType() override;

};
