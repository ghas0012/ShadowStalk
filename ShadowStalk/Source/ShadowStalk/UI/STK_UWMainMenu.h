// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Arianne Fennell

// Changelog:
// - Class init.

#pragma once

#include "CoreMinimal.h"
#include "STK_UserWidget.h"
#include "STK_UWMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWSTALK_API USTK_UWMainMenu : public USTK_UserWidget
{
	GENERATED_BODY()

public:
	USTK_UWMainMenu();

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UButton* PlayButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UButton* OptionsButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UButton* CreditsButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UButton* QuitButton = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "SoundFX")
	class USoundBase* HoverSoundFX;

protected:

	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:

	UFUNCTION()
	void PlayPressed();

	UFUNCTION()
	void OpenOptionsMenu();

	UFUNCTION()
	void OpenCreditsPanel();

	UFUNCTION()
	void QuitPressed();

	UFUNCTION()
	void PlayerHoverSoundFX();
};
