// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Arianne Fennell
  Date Modified: 3/22/2021
  Comment/Description: ShadowStalk base user widget parent.
  ChangeLog:
  A 3/12/2021: Implemented basic play and quit button functionality.
  A 3/22/2021: Added functions to call UMG animations from C++.
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STK_UserWidget.generated.h"

UCLASS(Abstract)
class SHADOWSTALK_API USTK_UserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    USTK_UserWidget(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(BlueprintReadOnly, Category = "SoundFX")
    class USoundBase* HoverSoundFX;

public:

    //Optionally override the Blueprint "Event Construct" event
    virtual void NativeConstruct() override;

    // Optionally override the tick event
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    void Setup();
    void Teardown();

    UWidgetAnimation* GetAnimationByName(FName AnimationName) const;

    bool PlayAnimationByName(FName AnimationName,
        float StartAtTime,
        int32 NumLoopsToPlay,
        EUMGSequencePlayMode::Type PlayMode,
        float PlaybackSpeed);

    UFUNCTION()
    void PlayHoverSoundFX();

protected:
    
    TMap<FName, UWidgetAnimation*> AnimationsMap;

    void FillAnimationsMap();
};
