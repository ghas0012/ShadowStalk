// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_UWMainMenu.h"

#include "UObject/ConstructorHelpers.h"
#include "ShadowStalk/UI/STK_UWCreditsPanel.h"
#include "ShadowStalk/UI/STK_UWOptionsPanel.h"
#include "ShadowStalk/GameInstance/STK_GameInstance.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

USTK_UWMainMenu::USTK_UWMainMenu(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer)
{
    //Search for Options Panel Widget Blueprint
    {
        ConstructorHelpers::FClassFinder<UUserWidget> OptionsPanelBPClass(TEXT("/Game/UI/WBP_OptionsPanel"));
        if (!ensure(OptionsPanelBPClass.Class != nullptr)) return;

        OptionsPanelClass = OptionsPanelBPClass.Class;
    }

    //Search for Credits Panel Widget Blueprint
    {
        ConstructorHelpers::FClassFinder<UUserWidget> CreditsPanelBPClass(TEXT("/Game/UI/WBP_CreditsPanel"));
        if (!ensure(CreditsPanelBPClass.Class != nullptr)) return;

        CreditsPanelClass = CreditsPanelBPClass.Class;
    }
}

bool USTK_UWMainMenu::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) return false;

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

#ifdef STEAM_ENABLED

    bool Success = GameInstance->HostSession("Game", "MapTest", false, true, 5);

    if (Success)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Created Session"));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Failed To Create Session"));
    }

#else

    GameInstance->LoadGameLevel();

#endif

}

/// <summary>
/// Creates and sets up the Options Panel Widget in the game's viewport.
/// </summary>
void USTK_UWMainMenu::OpenOptionsMenu()
{
    if (!ensure(OptionsPanelClass != nullptr)) return;

    UWOptionsPanel = CreateWidget<USTK_UWOptionsPanel>(this, OptionsPanelClass);
    if (!ensure(UWOptionsPanel != nullptr)) return;

    UWOptionsPanel->Setup();
}

/// <summary>
/// Creates and sets up the Credits Panel Widget in the game's viewport.
/// </summary>
void USTK_UWMainMenu::OpenCreditsPanel()
{
    if (!ensure(CreditsPanelClass != nullptr)) return;

    UWCreditsPanel = CreateWidget<USTK_UWCreditsPanel>(this, CreditsPanelClass);
    if (!ensure(UWCreditsPanel != nullptr)) return;

    UWCreditsPanel->Setup();
}

/// <summary>
/// Allows the player to exit and close the game window.
/// </summary>
void USTK_UWMainMenu::QuitPressed()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
