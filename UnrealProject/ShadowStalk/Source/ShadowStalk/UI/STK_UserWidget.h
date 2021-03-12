// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

 /*
  Author: Arianne Fennell
  Date Modified: 3/12/2021
  Comment/Description: ShadowStalk base user widget parent.
  ChangeLog:
  3/12/2021: Implemented basic play and quit button functionality.
  */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STK_UserWidget.generated.h"

UCLASS()
class SHADOWSTALK_API USTK_UserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup();
	void Teardown();
};
