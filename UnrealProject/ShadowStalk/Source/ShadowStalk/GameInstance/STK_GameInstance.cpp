// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_GameInstance.h"

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "ShadowStalk/UI/STK_UserWidget.h"
#include "ShadowStalk/UI/STK_UWMainMenu.h"
#include "ShadowStalk/UI/STK_UWPauseMenu.h"

USTK_GameInstance::USTK_GameInstance(const FObjectInitializer& ObjectInitializer)
{
    //Search for Main Menu Widget Blueprint
    {
        ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/UI/WBP_MainMenu"));
        if (!ensure(MainMenuBPClass.Class != nullptr)) return;

        MainMenuClass = MainMenuBPClass.Class;
    }

    //Search for Pause Menu Widget Blueprint
    {
        ConstructorHelpers::FClassFinder<UUserWidget> PauseMenuBPClass(TEXT("/Game/UI/WBP_PauseMenu"));
        if (!ensure(PauseMenuBPClass.Class != nullptr)) return;

        PauseMenuClass = PauseMenuBPClass.Class;
    }
}

/// <summary>
/// Creates and sets up the Main Menu Widget in the game's viewport.
/// </summary>
void USTK_GameInstance::SetupMainMenuWidget()
{
    if (!ensure(MainMenuClass != nullptr)) return;

    UWMainMenu = CreateWidget<USTK_UWMainMenu>(this, MainMenuClass);
    if (!ensure(UWMainMenu != nullptr)) return;

    UWMainMenu->Setup();
}

/// <summary>
/// Creates and sets up the Pause Menu Widget in the game's viewport.
/// </summary>
void USTK_GameInstance::SetupPauseMenuWidget()
{
    if (!ensure(PauseMenuClass != nullptr)) return;

    UWPauseMenu = CreateWidget<USTK_UWPauseMenu>(this, PauseMenuClass);
    if (!ensure(UWPauseMenu != nullptr)) return;

    UWPauseMenu->Setup();
}

/// <summary>
/// Removes the Main Menu Widget from the game and loads the game's default level.
/// </summary>
void USTK_GameInstance::LoadGameLevel()
{
    UWorld* World = GetWorld();
    if (!ensure(World != nullptr)) return;

    if (!ensure(UWMainMenu != nullptr)) return;
    UWMainMenu->Teardown();

    UGameplayStatics::OpenLevel(World, "MapTest");
}





