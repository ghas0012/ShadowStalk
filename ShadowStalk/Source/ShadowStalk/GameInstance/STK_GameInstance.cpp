// Fill out your copyright notice in the Description page of Project Settings.


#include "STK_GameInstance.h"

#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ShadowStalk/UI/STK_UserWidget.h"
#include "ShadowStalk/UI/STK_UWMainMenu.h"

USTK_GameInstance::USTK_GameInstance(const FObjectInitializer& ObjectInitializer)
{
    ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/UI/WBP_MainMenu"));
    if (!ensure(MainMenuBPClass.Class != nullptr)) return;

    MainMenuClass = MainMenuBPClass.Class;
}

void USTK_GameInstance::LoadMainMenuWidget()
{
    if (!ensure(MainMenuClass != nullptr)) return;

    UWMainMenu = CreateWidget<USTK_UWMainMenu>(this, MainMenuClass);
    if (!ensure(UWMainMenu != nullptr)) return;

    UWMainMenu->Setup();
}

void USTK_GameInstance::LoadGameLevel()
{
    UWorld* World = GetWorld();
    if (!ensure(World != nullptr)) return;

    if (!ensure(UWMainMenu != nullptr)) return;
    UWMainMenu->Teardown();

    UGameplayStatics::OpenLevel(World, "MapTest");
}

