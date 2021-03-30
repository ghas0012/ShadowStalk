// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

 /*
  Author: Arianne Fennell
  Date Modified: 3/23/2021
  Comment/Description: Base user widget responsible for the Pause Menu interface.
  ChangeLog:
  A 3/23/2021: Implemented basic interface with button functionality.
  */

#pragma once

#include "CoreMinimal.h"
#include "STK_UserWidget.h"
#include "STK_UWPauseMenu.generated.h"

UCLASS()
class SHADOWSTALK_API USTK_UWPauseMenu : public USTK_UserWidget
{
    GENERATED_BODY()

public:
    USTK_UWPauseMenu(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UImage* BGOverlay = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* ContinueButton = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* OptionsButton = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* QuitButton = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* ExitButton = nullptr;

protected:

    virtual bool Initialize() override;

private:

    TSubclassOf<class UUserWidget> OptionsPanelClass;
    class USTK_UWOptionsPanel* UWOptionsPanel;

    UFUNCTION()
    void ExitPauseMenu();

    UFUNCTION()
    void OpenOptionsMenu();

    UFUNCTION()
    void ReturnToMainMenu();
};
