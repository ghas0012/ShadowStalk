// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STK_UserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWSTALK_API USTK_UserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup();
	void Teardown();
};
