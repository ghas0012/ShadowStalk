// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_UWMainMenu.h"

#include "ShadowStalk/GameInstance/STK_GameInstance.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"

bool USTK_UWMainMenu::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) return false;

    if (!ensure(MainMenuButtons != nullptr)) return false;

    //Setup Input for Play Button
    if (!ensure(PlayButton != nullptr)) return false;
    PlayButton->OnClicked.AddDynamic(this, &USTK_UWMainMenu::PlayPressed);
    PlayButton->OnHovered.AddDynamic(this, &USTK_UserWidget::PlayHoverSoundFX);

    //Setup Input for Options Button
    if (!ensure(OptionsButton != nullptr)) return false;
    OptionsButton->OnClicked.AddDynamic(this, &USTK_UWMainMenu::OpenOptionsMenu);
    OptionsButton->OnHovered.AddDynamic(this, &USTK_UserWidget::PlayHoverSoundFX);

    //Setup Input for Credits Button
    if (!ensure(CreditsButton != nullptr)) return false;
    CreditsButton->OnClicked.AddDynamic(this, &USTK_UWMainMenu::OpenCreditsPanel);
    CreditsButton->OnHovered.AddDynamic(this, &USTK_UserWidget::PlayHoverSoundFX);

    //Setup Input for Quit Button
    if (!ensure(QuitButton != nullptr)) return false;
    QuitButton->OnClicked.AddDynamic(this, &USTK_UWMainMenu::QuitPressed);
    QuitButton->OnHovered.AddDynamic(this, &USTK_UserWidget::PlayHoverSoundFX);

    return true;
}

/// <summary>
/// Allows the player to enter the game level map.
/// </summary>
void USTK_UWMainMenu::PlayPressed()
{
    auto GameInstance = Cast<USTK_GameInstance>(GetGameInstance());
    if (GameInstance == nullptr) return;

    GameInstance->LoadGameLevel();
}

void USTK_UWMainMenu::OpenOptionsMenu()
{
    //TODO: Inlcude Options for keybindings, resolutions, etc.

    auto GameInstance = Cast<USTK_GameInstance>(GetGameInstance());
    if (GameInstance == nullptr) return;

    GameInstance->SetupOptionsWidget();
}

void USTK_UWMainMenu::OpenCreditsPanel()
{
    auto GameInstance = Cast<USTK_GameInstance>(GetGameInstance());
    if (GameInstance == nullptr) return;

    GameInstance->SetupCreditsWidget();
}

/// <summary>
/// Allows the player to exit and close the game window.
/// </summary>
void USTK_UWMainMenu::QuitPressed()
{
    UWorld* World = GetWorld();
    if (!ensure(World != nullptr)) return;

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    PlayerController->ConsoleCommand("quit");
}
