// Fill out your copyright notice in the Description page of Project Settings.


#include "STK_UserWidget.h"

void USTK_UserWidget::Setup()
{
    this->AddToViewport();

    UWorld* World = GetWorld();
    if (!ensure(World != nullptr)) return;

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    //Set the input method for navigating the UI
    FInputModeUIOnly InputModeData;
    InputModeData.SetWidgetToFocus(this->TakeWidget());
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = true;
}

void USTK_UserWidget::Teardown()
{
    this->RemoveFromViewport();

    UWorld* World = GetWorld();
    if (!ensure(World != nullptr)) return;

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    FInputModeGameOnly InputModeData;

    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = false;
}
