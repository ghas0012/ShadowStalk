// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_UserWidget.h"

#include "UObject/UObjectGlobals.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/Button.h"
#include "Animation/WidgetAnimation.h"

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

void USTK_UserWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void USTK_UserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
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

void USTK_UserWidget::FillAnimationsMap()
{
    AnimationsMap.Empty();

    FProperty* Property = GetClass()->PropertyLink;

    while (Property != nullptr)
    {
        if (Property->GetClass() == FObjectProperty::StaticClass())
        {
            FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property);

            if (ObjectProperty->PropertyClass == UWidgetAnimation::StaticClass())
            {
                UObject* Object = ObjectProperty->GetObjectPropertyValue_InContainer(this);

                UWidgetAnimation* WidgetAnim = Cast<UWidgetAnimation>(Object);

                if (WidgetAnim != nullptr && WidgetAnim->MovieScene != nullptr)
                {
                    FName AnimName = WidgetAnim->MovieScene->GetFName();
                    AnimationsMap.Add(AnimName, WidgetAnim);
                }
            }
        }

        Property = Property->PropertyLinkNext;
    }
}

UWidgetAnimation* USTK_UserWidget::GetAnimationByName(FName AnimationName) const
{
    UWidgetAnimation* const* WidgetAnim = AnimationsMap.Find(AnimationName);
    if (WidgetAnim)
    {
        return *WidgetAnim;
    }
    return nullptr;
}

bool USTK_UserWidget::PlayAnimationByName(FName AnimationName, 
    float StartAtTime, 
    int32 NumLoopsToPlay, 
    EUMGSequencePlayMode::Type PlayMode, 
    float PlaybackSpeed)
{
    UWidgetAnimation* WidgetAnim = GetAnimationByName(AnimationName);
    if (WidgetAnim)
    {
        PlayAnimation(WidgetAnim, StartAtTime, NumLoopsToPlay, PlayMode, PlaybackSpeed);
        return true;
    }
    return false;
}

/// <summary>
/// Plays a sound effect on button hover.
/// </summary>
void USTK_UserWidget::PlayHoverSoundFX()
{
    UGameplayStatics::PlaySound2D(GetWorld(), HoverSoundFX);
}





