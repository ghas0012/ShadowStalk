// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Arianne Fennell

// Changelog:
// - Class init.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STK_UserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWSTALK_API USTK_UserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup();
	void Teardown();

	//void SetMenuInterface(IMenuInterface* MenuInterface);

protected:
	//IMenuInterface* MenuInterface;
};
