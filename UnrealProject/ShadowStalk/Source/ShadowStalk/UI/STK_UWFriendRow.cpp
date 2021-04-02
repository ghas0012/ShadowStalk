// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_UWFriendRow.h"

#include "STK_UWFriendsList.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ShadowStalk/GameInstance/STK_GameInstance.h"
#include "Kismet/GameplayStatics.h"

void USTK_UWFriendRow::Setup(USTK_UWFriendsList* InParent, uint32 InIndex)
{
    Parent = InParent;
    Index = InIndex;

    //Setup Input for Row Button
    RowButton->OnClicked.AddDynamic(this, &USTK_UWFriendRow::OnClicked);
    RowButton->OnHovered.AddDynamic(this, &USTK_UserWidget::PlayHoverSoundFX);
}

void USTK_UWFriendRow::OnClicked()
{
    auto GameInstance = Cast<USTK_GameInstance>(GetGameInstance());

    if (GameInstance)
    {
        GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, "Inviting Friend --- " + DisplayName->Text.ToString());

        //Sends an invite to the selected friend.
        GameInstance->SendSessionInviteToFriend(UniqueNetId->GetText().ToString());
    }
}
