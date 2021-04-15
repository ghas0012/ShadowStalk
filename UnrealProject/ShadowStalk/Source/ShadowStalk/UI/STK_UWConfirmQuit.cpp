// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_UWConfirmQuit.h"

#include "ShadowStalk/UI/STK_UserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "ShadowStalk/GameInstance/STK_GameInstance.h"

USTK_UWConfirmQuit::USTK_UWConfirmQuit(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer)
{

}

bool USTK_UWConfirmQuit::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) return false;

    // Setup Input for Yes Button
    if (!ensure(YesButton != nullptr)) return false;
    YesButton->OnClicked.AddDynamic(this, &USTK_UWConfirmQuit::ReturnToMainMenu);
    YesButton->OnHovered.AddDynamic(this, &USTK_UserWidget::PlayHoverSoundFX);

    // Setup Input for No Button
    if (!ensure(NoButton != nullptr)) return false;
    NoButton->OnClicked.AddDynamic(this, &USTK_UWConfirmQuit::ExitDialogueBox);
    NoButton->OnHovered.AddDynamic(this, &USTK_UserWidget::PlayHoverSoundFX);

    // Setup Input for Exit Button
    if (!ensure(ExitButton != nullptr)) return false;
    ExitButton->OnClicked.AddDynamic(this, &USTK_UWConfirmQuit::ExitDialogueBox);
    ExitButton->OnHovered.AddDynamic(this, &USTK_UserWidget::PlayHoverSoundFX);

    return true;
}

/// <summary>
/// Enables the player to quit the session and return to the main menu.
/// </summary>
void USTK_UWConfirmQuit::ReturnToMainMenu()
{
    this->Teardown();

    auto GameInstance = Cast<USTK_GameInstance>(GetGameInstance());
    if (GameInstance == nullptr) return;

    GameInstance->DestroySession();

    GameInstance->SetupMainMenuWidget();

    UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}

void USTK_UWConfirmQuit::ExitDialogueBox()
{
    this->Teardown();

    auto GameInstance = Cast<USTK_GameInstance>(GetGameInstance());
    if (GameInstance == nullptr) return;

    GameInstance->SetupPauseMenuWidget();
}
