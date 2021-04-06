// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

 /*
  Author: Arianne Fennell
  Date Modified: 4/06/2021
  Comment/Description: Base user widget responsible for the displaying the Host's friends list.
  ChangeLog:
  A 3/30/2021: Implemented base class and functionality responsible for filling the user's visible friends list.
  A 4/06/2021: Removed code that sets the friends "RealName" in the FriendsRow widget.
  */

#pragma once

#include "CoreMinimal.h"
#include "STK_UserWidget.h"
#include "ShadowStalk/GameInstance/STK_GameInstance.h"
#include "STK_UWFriendsList.generated.h"

UCLASS()
class SHADOWSTALK_API USTK_UWFriendsList : public USTK_UserWidget
{
    GENERATED_BODY()

public:

    USTK_UWFriendsList(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UPanelWidget* FriendList;

    UFUNCTION()
    void SetFriendsList(TArray<FFriendData> Friends);

protected:

    TSubclassOf<class UUserWidget> FriendRowClass;

    virtual bool Initialize() override;
};
