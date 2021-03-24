// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_GameInstance.h"

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "ShadowStalk/UI/STK_UserWidget.h"
#include "ShadowStalk/UI/STK_UWMainMenu.h"

USTK_GameInstance::USTK_GameInstance(const FObjectInitializer& ObjectInitializer)
{
    //Search for Main Menu Widget Blueprint
    {
        ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/UI/WBP_MainMenu"));
        if (!ensure(MainMenuBPClass.Class != nullptr)) return;

        MainMenuClass = MainMenuBPClass.Class;
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

