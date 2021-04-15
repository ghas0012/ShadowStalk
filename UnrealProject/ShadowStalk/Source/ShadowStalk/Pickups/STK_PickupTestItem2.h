// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

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
