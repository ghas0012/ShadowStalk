// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_UWFriendsList.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "STK_UWFriendRow.h"

USTK_UWFriendsList::USTK_UWFriendsList(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer)
{
    //Search for Friend Row Widget Blueprint
    {
        ConstructorHelpers::FClassFinder<UUserWidget> FriendRowBPClass(TEXT("/Game/UI/WBP_FriendRow"));
        if (!ensure(FriendRowBPClass.Class != nullptr)) return;

        FriendRowClass = FriendRowBPClass.Class;
    }
}

bool USTK_UWFriendsList::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) return false;

    auto GameInstance = Cast<USTK_GameInstance>(GetGameInstance());

    if (GameInstance)
        SetFriendsList(GameInstance->GetFriends());

    return Success;
}

/// <summary>
/// Sets up a list of the user's friends that can be invited to the session.
/// </summary>
/// <param name="Friends">List of users friends accompanied by accessible data.</param>
void USTK_UWFriendsList::SetFriendsList(TArray<FFriendData> Friends)
{
    UWorld* World = this->GetWorld();
    if (!ensure(World != nullptr)) return;

    //Initialize by clearing friends list on setup.
    FriendList->ClearChildren();

    uint32 i = 0;
    for (const FFriendData& FriendData : Friends)
    {
        USTK_UWFriendRow* Row = CreateWidget<USTK_UWFriendRow>(World, FriendRowClass);
        if (!ensure(Row != nullptr)) return;

        //Set the rows text variables based on friend data
        Row->DisplayName->SetText(FText::FromString(FriendData.DisplayName));
        Row->Presence->SetText(FText::FromString(FriendData.Presence));
        Row->UniqueNetId->SetText(FText::FromString(FriendData.UniqueNetId));

        Row->Setup(this, i);

        ++i;

        FriendList->AddChild(Row);
    }
}


