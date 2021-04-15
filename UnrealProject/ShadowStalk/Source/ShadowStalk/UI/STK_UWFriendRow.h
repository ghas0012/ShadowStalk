// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

 /*
  Author: Arianne Fennell
  Date Modified: 4/06/2021
  Comment/Description: Base user widget responsible for sending out game invitations.
  ChangeLog:
  A 3/30/2021: Implemented base class and functionality responsible for sending a game invite to a friend.
  A 4/06/2021: Removed "RealName" UPROPERTY from FriendRow widget. 
  */

#pragma once

#include "CoreMinimal.h"
#include "STK_UserWidget.h"
#include "STK_UWFriendRow.generated.h"

UCLASS()
class SHADOWSTALK_API USTK_UWFriendRow : public USTK_UserWidget
{
    GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* DisplayName = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* Presence = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* UniqueNetId = nullptr;

    UPROPERTY(BlueprintReadOnly)
    bool bSelected = false;

    void Setup(class USTK_UWFriendsList* InParent, uint32 InIndex);

private:

    UPROPERTY(meta = (BindWidget))
    class UButton* RowButton;

    UPROPERTY()
    class USTK_UWFriendsList* Parent;

    uint32 Index;

    UFUNCTION()
    void OnClicked();
};
