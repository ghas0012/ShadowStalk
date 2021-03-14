// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

 /*
  Author: Arianne Fennell
  Date Modified: 3/12/2021
  Comment/Description: Base user widget responsible for the Credits panel of the Main Menu screen.
  ChangeLog:
  A 3/13/2021: Implemented basic credits panel class.
  */

#pragma once

#include "CoreMinimal.h"
#include "STK_UserWidget.h"
#include "STK_UWCreditsPanel.generated.h"

UCLASS()
class SHADOWSTALK_API USTK_UWCreditsPanel : public USTK_UserWidget
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
