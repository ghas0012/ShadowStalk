// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EntityMonsterController.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWSTALK_API AEntityMonsterController : public APlayerController
{
	GENERATED_BODY()
	
public:

	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

protected: 

	virtual void SetupInputComponent() override;

	void Forward(float value);
	void Strafe(float value);
	void Jump();
	void Interact();
	void MouseLook_Vertical(float value);
	void MouseLook_Horizontal(float value);

	class AEntityMonster* m_MonsterEntity;
};
