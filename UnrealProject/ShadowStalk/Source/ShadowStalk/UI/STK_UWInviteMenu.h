// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

 /*
  Author: Arianne Fennell
  Date Modified: 3/30/2021
  Comment/Description: Base user widget responsible for the in-game invite menu.
  ChangeLog:
  A 3/30/2021: Implemented basic invite menu class.
  */

#pragma once

#include "CoreMinimal.h"
#include "STK_UserWidget.h"
#include "STK_UWInviteMenu.generated.h"

UCLASS()
class SHADOWSTALK_API USTK_UWInviteMenu : public USTK_UserWidget
{
    GENERATED_BODY()

public:

    USTK_UWInviteMenu(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UImage* BGOverlay = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* ExitButton = nullptr;

protected:
    virtual bool Initialize() override;

private:

    UFUNCTION()
    void ExitPressed();
};
