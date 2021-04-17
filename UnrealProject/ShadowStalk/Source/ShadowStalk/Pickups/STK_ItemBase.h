// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Jeffrey Armstrong
  Date Modified: 4/14/2021
  Comment/Description:

	Base class for items in the game.
	It identifies the item type, and inherits from STK_PickupBase so that all of the logic from that applies to all of the item types here.

  ChangeLog:
  J 4/14/2021: Class init.
  J 4/15/2021: Added a class description and summaries to relevant methods.

*/

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
