// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Arianne Fennell

// Changelog:
// - Class init.

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

    if (!ensure(PlayButton != nullptr)) return false;
    PlayButton->OnClicked.AddDynamic(this, &USTK_UWMainMenu::PlayPressed);
    PlayButton->OnHovered.AddDynamic(this, &USTK_UWMainMenu::PlayHoverSoundFX);

    if (!ensure(OptionsButton != nullptr)) return false;
    OptionsButton->OnClicked.AddDynamic(this, &USTK_UWMainMenu::OpenOptionsMenu);
    OptionsButton->OnHovered.AddDynamic(this, &USTK_UWMainMenu::PlayHoverSoundFX);

    if (!ensure(CreditsButton != nullptr)) return false;
    CreditsButton->OnClicked.AddDynamic(this, &USTK_UWMainMenu::OpenCreditsPanel);
    CreditsButton->OnHovered.AddDynamic(this, &USTK_UWMainMenu::PlayHoverSoundFX);

    if (!ensure(QuitButton != nullptr)) return false;
    QuitButton->OnClicked.AddDynamic(this, &USTK_UWMainMenu::QuitPressed);
    QuitButton->OnHovered.AddDynamic(this, &USTK_UWMainMenu::PlayHoverSoundFX);

    return true;
}

void USTK_UWMainMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}

void USTK_UWMainMenu::PlayPressed()
{
    auto GameInstance = Cast<USTK_GameInstance>(GetGameInstance());

    if (GameInstance == nullptr) return;

    GameInstance->LoadGameLevel();
}

void USTK_UWMainMenu::OpenOptionsMenu()
{
    //TODO - Open options menu (resolution, keybindings, etc);
}

void USTK_UWMainMenu::OpenCreditsPanel()
{
    //TODO - Open credits window

}

void USTK_UWMainMenu::QuitPressed()
{
    UWorld* World = GetWorld();
    if (!ensure(World != nullptr)) return;

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    PlayerController->ConsoleCommand("quit");
}

void USTK_UWMainMenu::PlayHoverSoundFX()
{
    UGameplayStatics::PlaySound2D(GetWorld(), HoverSoundFX);
}
