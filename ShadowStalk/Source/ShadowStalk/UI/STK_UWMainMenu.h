// Fill out your copyright notice in the Description page of Project Settings.

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
	class UImage* GameTitleImage = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UVerticalBox* MainMenuButtons = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* PlayButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* OptionsButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* CreditsButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
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
	void PlayHoverSoundFX();
};
