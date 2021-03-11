// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Christian Young

// Changelog:
// - Class init.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STK_EntityMonsterController.generated.h"

/**
 *
 */
UCLASS()
class SHADOWSTALK_API ASTK_EntityMonsterController : public APlayerController
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
	void Attack();
	void SetSprint();
	void StopSprint();
	void MouseLook_Vertical(float value);
	void MouseLook_Horizontal(float value);

	class ASTK_EntityMonster* m_MonsterEntity;
};
