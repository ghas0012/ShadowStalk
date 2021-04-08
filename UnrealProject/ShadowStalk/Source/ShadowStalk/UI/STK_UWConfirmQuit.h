// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

 /*
  Author: Arianne Fennell
  Date Modified: 4/07/2021
  Comment/Description: Base user widget responsible for the Quit confirmation dialogue box.
  ChangeLog:
  A 4/07/2021: Implemented basic interface with button functionality.
  */

#pragma once

#include "CoreMinimal.h"
#include "STK_UserWidget.h"
#include "STK_UWConfirmQuit.generated.h"

UCLASS()
class SHADOWSTALK_API USTK_UWConfirmQuit : public USTK_UserWidget
{
    GENERATED_BODY()

public:
    USTK_UWConfirmQuit(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* YesButton = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* NoButton = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* ExitButton = nullptr;

protected:

    virtual bool Initialize() override;

private:

    UFUNCTION()
    void ReturnToMainMenu();

    UFUNCTION()
    void ExitDialogueBox();
};
