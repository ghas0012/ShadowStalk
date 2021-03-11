// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Jeff Armstrong

// Changelog:
// - Class init.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STK_EntityShadeController.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWSTALK_API ASTK_EntityShadeController : public APlayerController
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
	void SetSprint();
	void StopSprint();
	void SetCrawl();
	void UnsetCrawl();
	void Interact();
	void MouseLook_Vertical(float value);
	void MouseLook_Horizontal(float value);

	class ASTK_EntityShade* m_ShadeEntity;
};
