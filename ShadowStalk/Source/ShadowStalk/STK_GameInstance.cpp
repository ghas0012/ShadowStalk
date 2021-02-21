// Fill out your copyright notice in the Description page of Project Settings.


#include "STK_GameInstance.h"

#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "UI/STK_UserWidget.h"
#include "UI/STK_UWMainMenu.h"

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
