// Fill out your copyright notice in the Description page of Project Settings.

#include "STK_UWMainMenu.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/Button.h"

bool USTK_UWMainMenu::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) return false;

    if (!ensure(PlayButton != nullptr)) return false;
    PlayButton->OnClicked.AddDynamic(this, &USTK_UWMainMenu::PlayPressed);

    if (!ensure(OptionsButton != nullptr)) return false;
    OptionsButton->OnClicked.AddDynamic(this, &USTK_UWMainMenu::OpenOptionsMenu);

    if (!ensure(CreditsButton != nullptr)) return false;
    CreditsButton->OnClicked.AddDynamic(this, &USTK_UWMainMenu::OpenCreditsPanel);

    if (!ensure(QuitButton != nullptr)) return false;
    QuitButton->OnClicked.AddDynamic(this, &USTK_UWMainMenu::QuitPressed);

    return true;
}

void USTK_UWMainMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}

void USTK_UWMainMenu::PlayPressed()
{
    //TODO: Load test game map on button click.
}

void USTK_UWMainMenu::OpenOptionsMenu()
{
    //TODO: Open options menu (resolution, keybindings, etc);
}

void USTK_UWMainMenu::OpenCreditsPanel()
{
    //TODO: Open credits panel
}

void USTK_UWMainMenu::QuitPressed()
{
    UWorld* World = GetWorld();
    if (!ensure(World != nullptr)) return;

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    PlayerController->ConsoleCommand("quit");
}
