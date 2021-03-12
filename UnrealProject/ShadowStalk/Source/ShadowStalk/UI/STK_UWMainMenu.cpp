// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_UWMainMenu.h"

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "ShadowStalk/GameInstance/STK_GameInstance.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Sound/SoundBase.h"

USTK_UWMainMenu::USTK_UWMainMenu()
{
    //Find and set HoverSoundFX
    ConstructorHelpers::FObjectFinder<USoundBase> HoverSound(TEXT("/Game/Custom_Audio/LightFlicker"));
    if (!ensure(HoverSound.Object != nullptr)) return;

    HoverSoundFX = HoverSound.Object;
}

bool USTK_UWMainMenu::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) return false;

    if (!ensure(GameTitleImage != nullptr)) return false;
    if (!ensure(MainMenuButtons != nullptr)) return false;

    //Setup Input for Play Button
    if (!ensure(PlayButton != nullptr)) return false;
    PlayButton->OnClicked.AddDynamic(this, &USTK_UWMainMenu::PlayPressed);
    PlayButton->OnHovered.AddDynamic(this, &USTK_UWMainMenu::PlayHoverSoundFX);

    //Setup Input for Options Button
    if (!ensure(OptionsButton != nullptr)) return false;
    OptionsButton->OnClicked.AddDynamic(this, &USTK_UWMainMenu::OpenOptionsMenu);
    OptionsButton->OnHovered.AddDynamic(this, &USTK_UWMainMenu::PlayHoverSoundFX);

    //Setup Input for Credits Button
    if (!ensure(CreditsButton != nullptr)) return false;
    CreditsButton->OnClicked.AddDynamic(this, &USTK_UWMainMenu::OpenCreditsPanel);
    CreditsButton->OnHovered.AddDynamic(this, &USTK_UWMainMenu::PlayHoverSoundFX);

    //Setup Input for Quit Button
    if (!ensure(QuitButton != nullptr)) return false;
    QuitButton->OnClicked.AddDynamic(this, &USTK_UWMainMenu::QuitPressed);
    QuitButton->OnHovered.AddDynamic(this, &USTK_UWMainMenu::PlayHoverSoundFX);

    return true;
}

void USTK_UWMainMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
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
    //TODO: Open options menu (resolution, keybindings, etc);
}

void USTK_UWMainMenu::OpenCreditsPanel()
{
    //TODO: Open credits window
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

/// <summary>
/// Plays a sound effect on button hover.
/// </summary>
void USTK_UWMainMenu::PlayHoverSoundFX()
{
    UGameplayStatics::PlaySound2D(GetWorld(), HoverSoundFX);
}
