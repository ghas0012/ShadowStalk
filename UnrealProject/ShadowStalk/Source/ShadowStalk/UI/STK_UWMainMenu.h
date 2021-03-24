// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

 /*
  Author: Arianne Fennell
  Date Modified: 3/12/2021
  Comment/Description: Base user widget responsible for the Main Menu interface.
  ChangeLog:
  A 3/12/2021: Implemented basic play and quit button functionality.
  */

#pragma once

#include "CoreMinimal.h"
#include "ShadowStalk/UI/STK_UserWidget.h"
#include "STK_UWMainMenu.generated.h"

UCLASS()
class SHADOWSTALK_API USTK_UWMainMenu : public USTK_UserWidget
{
    GENERATED_BODY()

public:
    USTK_UWMainMenu(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* PlayButton = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* OptionsButton = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* CreditsButton = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* QuitButton = nullptr;

protected:

    virtual bool Initialize() override;

private:

    TSubclassOf<class UUserWidget> OptionsPanelClass;
    class USTK_UWOptionsPanel* UWOptionsPanel;

    TSubclassOf<class UUserWidget> CreditsPanelClass;
    class USTK_UWCreditsPanel* UWCreditsPanel;

    UFUNCTION()
    void PlayPressed();

    UFUNCTION()
    void OpenOptionsMenu();

    UFUNCTION()
    void OpenCreditsPanel();

    UFUNCTION()
    void QuitPressed();
};
