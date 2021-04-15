// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STK_PickupBase.h"
#include "STK_ItemBase.generated.h"


UCLASS()
class SHADOWSTALK_API ASTK_ItemBase : public ASTK_PickupBase
{
	GENERATED_BODY()
	
public:
	virtual EPickupType GetPickupType() override;
	virtual EItemType GetItemType() { return EItemType::Undefined; };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pickup Base")
		EItemType ItemType;
};
