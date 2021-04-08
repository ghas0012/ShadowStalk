// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

 /*
  Author: Arianne Fennell
  Date Modified: 4/07/2021
  Comment/Description: Base user widget responsible for the Pause Menu interface.
  ChangeLog:
  A 3/23/2021: Implemented basic interface with button functionality.
  A 4/07/2021: Altered Pause Menu "Quit" functionality to open a dialogue box that confirms user action.
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
    class UButton* InviteButton = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* OptionsButton = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* QuitButton = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* ExitButton = nullptr;

protected:

    virtual bool Initialize() override;

private:

    TSubclassOf<class UUserWidget> InviteMenuClass;
    class USTK_UWInviteMenu* UWInviteMenu;

    TSubclassOf<class UUserWidget> OptionsPanelClass;
    class USTK_UWOptionsPanel* UWOptionsPanel;

    TSubclassOf<class UUserWidget> ConfirmQuitClass;
    class USTK_UWConfirmQuit* UWConfirmQuit;

    UFUNCTION()
    void OpenInviteMenu();

    UFUNCTION()
    void OpenOptionsMenu();

    UFUNCTION()
    void OpenConfirmQuitBox();

    UFUNCTION()
    void ReturnToMainMenu();

    UFUNCTION()
    void ExitPauseMenu();
};
