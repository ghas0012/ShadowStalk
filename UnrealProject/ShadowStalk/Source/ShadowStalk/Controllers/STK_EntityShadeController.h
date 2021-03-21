// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Christian Young, Jeffrey Armstrong
  Date Modified: 3/12/2021
  Comment/Description: A controller for the Shade pawn. Doesn't allow attacking.
  ChangeLog:
  J 3/12/2021: Initialized class and populated input.
  H 3/16/2021: Added a class description and summaries to pertinent functions.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STK_EntityShadeController.generated.h"

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
    //void StopSprint();
    void SetCrawl();
    void UnsetCrawl();
    void Interact();
    void MouseLook_Vertical(float value);
    void MouseLook_Horizontal(float value);

    bool isSprint;

    UPROPERTY(Replicated)
    class ASTK_EntityShade* m_ShadeEntity;
};
