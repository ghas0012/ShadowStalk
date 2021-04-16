// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Jeffrey Armstrong
  Date Modified: 4/14/2021
  Comment/Description:

	Inherits from STK_ItemBase. Item only meant for testing purposes.

  ChangeLog:
  J 4/14/2021: Class init.
  J 4/15/2021: Added a class description and summaries to relevant methods.

*/

#pragma once

#include "CoreMinimal.h"
#include "STK_ItemBase.h"
#include "STK_PickupTestItem2.generated.h"

UCLASS()
class SHADOWSTALK_API ASTK_PickupTestItem2 : public ASTK_ItemBase
{
	GENERATED_BODY()
	
public:	
	virtual EItemType GetItemType() override;
};
