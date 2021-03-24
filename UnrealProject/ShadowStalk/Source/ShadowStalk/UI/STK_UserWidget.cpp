// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_UserWidget.h"

//#include "UObject/UObjectGlobals.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/Button.h"

USTK_UserWidget::USTK_UserWidget(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer)
{
    //Find and set HoverSoundFX
    {
        ConstructorHelpers::FObjectFinder<USoundBase> HoverSound(TEXT("/Game/Custom_Audio/LightFlicker"));
        if (!ensure(HoverSound.Object != nullptr)) return;

        HoverSoundFX = HoverSound.Object;
    }
}

/// <summary>
/// Adds the specific User Widget to the viewport and modifies the player input for UI navigation.
/// </summary>
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

/// <summary>
/// Removes the specific User Widget from the viewport and modifies the player input for Game navigation.
/// </summary>
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

/// <summary>
/// Plays a sound effect on button hover.
/// </summary>
void USTK_UserWidget::PlayHoverSoundFX()
{
    UGameplayStatics::PlaySound2D(GetWorld(), HoverSoundFX);
}





