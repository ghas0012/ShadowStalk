// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

 /*
  Author: Arianne Fennell
  Date Modified: 3/12/2021
  Comment/Description: Base user widget responsible for the Options panel of the Main Menu screen.
  ChangeLog:
  A 3/13/2021: Implemented basic options panel class.
  */

#pragma once

#include "CoreMinimal.h"
#include "STK_UserWidget.h"
#include "STK_UWOptionsPanel.generated.h"

UCLASS()
class SHADOWSTALK_API USTK_UWOptionsPanel : public USTK_UserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* ExitButton = nullptr;

protected:

	virtual bool Initialize() override;

private:

	UFUNCTION()
	void ExitPressed();
};
