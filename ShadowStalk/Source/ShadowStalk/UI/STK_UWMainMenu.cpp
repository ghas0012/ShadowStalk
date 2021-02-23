// Fill out your copyright notice in the Description page of Project Settings.

#include "STK_UWMainMenu.h"

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "ShadowStalk/GameInstance/STK_GameInstance.h"
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

    if (!ensure(PlayButton != nullptr)) return false;
    PlayButton->OnClicked.AddDynamic(this, &USTK_UWMainMenu::PlayPressed);
    PlayButton->OnHovered.AddDynamic(this, &USTK_UWMainMenu::PlayerHoverSoundFX);

    if (!ensure(OptionsButton != nullptr)) return false;
    OptionsButton->OnClicked.AddDynamic(this, &USTK_UWMainMenu::OpenOptionsMenu);
    OptionsButton->OnHovered.AddDynamic(this, &USTK_UWMainMenu::PlayerHoverSoundFX);

    if (!ensure(CreditsButton != nullptr)) return false;
    CreditsButton->OnClicked.AddDynamic(this, &USTK_UWMainMenu::OpenCreditsPanel);
    CreditsButton->OnHovered.AddDynamic(this, &USTK_UWMainMenu::PlayerHoverSoundFX);

    if (!ensure(QuitButton != nullptr)) return false;
    QuitButton->OnClicked.AddDynamic(this, &USTK_UWMainMenu::QuitPressed);
    QuitButton->OnHovered.AddDynamic(this, &USTK_UWMainMenu::PlayerHoverSoundFX);

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
    //TODO - Open credits panel
}

void USTK_UWMainMenu::QuitPressed()
{
    UWorld* World = GetWorld();
    if (!ensure(World != nullptr)) return;

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    PlayerController->ConsoleCommand("quit");
}

void USTK_UWMainMenu::PlayerHoverSoundFX()
{
    UGameplayStatics::PlaySound2D(GetWorld(), HoverSoundFX);
}
