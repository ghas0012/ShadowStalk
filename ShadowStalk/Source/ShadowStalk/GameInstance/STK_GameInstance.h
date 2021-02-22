// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STK_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWSTALK_API USTK_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USTK_GameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void LoadMainMenuWidget();

	UFUNCTION(BlueprintCallable)
	void LoadGameLevel();

private:
	TSubclassOf<class UUserWidget> MainMenuClass;

	class USTK_UWMainMenu* UWMainMenu;
};
