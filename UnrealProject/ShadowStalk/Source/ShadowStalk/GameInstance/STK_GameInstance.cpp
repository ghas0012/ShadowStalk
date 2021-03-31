// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_GameInstance.h"

#include "Engine/GameEngine.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "ShadowStalk/UI/STK_UserWidget.h"
#include "ShadowStalk/UI/STK_UWMainMenu.h"
#include "ShadowStalk/UI/STK_UWPauseMenu.h"
#include "ShadowStalk/UI/STK_UWInviteMenu.h"

const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

USTK_GameInstance::USTK_GameInstance(const FObjectInitializer& ObjectInitializer)
{
    //Create the Delegates
    {
        OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &USTK_GameInstance::OnCreateSessionComplete);
        OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &USTK_GameInstance::OnStartSessionComplete);
        OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &USTK_GameInstance::OnFindSessionsComplete);
        OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &USTK_GameInstance::OnJoinSessionComplete);
        OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &USTK_GameInstance::OnDestroySessionComplete);
        OnReadFriendsListCompleteDelegate = FOnReadFriendsListComplete::CreateUObject(this, &USTK_GameInstance::OnReadFriendsListComplete);
        OnSessionUserInviteAcceptedDelegate = FOnSessionUserInviteAcceptedDelegate::CreateUObject(this, &USTK_GameInstance::OnSessionUserInviteAccepted);
    }

    //Search for Main Menu Widget Blueprint
    {
        ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/UI/WBP_MainMenu"));
        if (!ensure(MainMenuBPClass.Class != nullptr)) return;

        MainMenuClass = MainMenuBPClass.Class;
    }

    //Search for Pause Menu Widget Blueprint
    {
        ConstructorHelpers::FClassFinder<UUserWidget> PauseMenuBPClass(TEXT("/Game/UI/WBP_PauseMenu"));
        if (!ensure(PauseMenuBPClass.Class != nullptr)) return;

        PauseMenuClass = PauseMenuBPClass.Class;
    }

    //Search for Invite Menu Blueprint
    {
        ConstructorHelpers::FClassFinder<UUserWidget> InviteMenuBPClass(TEXT("/Game/UI/WBP_InviteMenu"));
        if (!ensure(InviteMenuBPClass.Class != nullptr)) return;

        InviteMenuClass = InviteMenuBPClass.Class;
    }
}

/// <summary>
/// Allows the Game Instance to setup what it needs.
/// </summary>
void USTK_GameInstance::Init()
{
    Super::Init();

    const IOnlineSubsystem* OnlineSubsystemInterface = IOnlineSubsystem::Get();

    if (OnlineSubsystemInterface)
    {
        IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystemInterface->GetSessionInterface();

        if (OnlineSessionInterface.IsValid())
        {
            //Initialize the join session complete delegate handle.
            OnJoinSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

            //Initialize the session user invite accepted delegate handle.
            OnSessionUserInviteAcceptedDelegateHandle = OnlineSessionInterface->AddOnSessionUserInviteAcceptedDelegate_Handle(OnSessionUserInviteAcceptedDelegate);
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsystem found!"));
    }
}

/// <summary>
/// Allows for the chance of cleanup when shutting down.
/// </summary>
void USTK_GameInstance::Shutdown()
{
    Super::Shutdown();

    const IOnlineSubsystem* OnlineSubsystemInterface = IOnlineSubsystem::Get();

    if (OnlineSubsystemInterface)
    {
        IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystemInterface->GetSessionInterface();

        if (OnlineSessionInterface.IsValid())
        {
            //Clear the session user invite accepted delegate.
            OnlineSessionInterface->ClearOnSessionUserInviteAcceptedDelegate_Handle(OnSessionUserInviteAcceptedDelegateHandle);
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsystem found!"));
    }
}

bool USTK_GameInstance::HostSession(FName SessionName, FName Map, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
    const ULocalPlayer* LocalPlayer = GetFirstGamePlayer();

    if (LocalPlayer->IsValidLowLevelFast())
    {
        FUniqueNetIdWrapper UniqueNetIdWrapper = FUniqueNetIdWrapper(LocalPlayer->GetPreferredUniqueNetId());

        //Create the session
        return CreateSession(UniqueNetIdWrapper.GetUniqueNetId(), SessionName, Map, bIsLAN, bIsPresence, MaxNumPlayers);
    }

    return false;
}

/// <summary>
/// Function to call create session.
/// </summary>
/// <param name="UserId">User that started the request.</param>
/// <param name="SessionName">Name of the session.</param>
/// <param name="Map">Name of the map.</param>
/// <param name="bIsLAN">Is this a LAN game?</param>
/// <param name="bIsPresence">Is the Session to create a presence session?</param>
/// <param name="MaxNumPlayers">Number of maximum allowed players on this session.</param>
/// <returns>True if successful, false otherwise.</returns>
bool USTK_GameInstance::CreateSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, FName Map,
    bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
    const IOnlineSubsystem* OnlineSubsystemInterface = IOnlineSubsystem::Get();

    if (OnlineSubsystemInterface)
    {
        IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystemInterface->GetSessionInterface();

        if (OnlineSessionInterface.IsValid() && UserId.IsValid())
        {
            //Set the sessions settings.
            SessionSettings = MakeShareable(new FOnlineSessionSettings());

            SessionSettings->bIsLANMatch = bIsLAN;
            SessionSettings->bUsesPresence = bIsPresence;
            SessionSettings->NumPublicConnections = MaxNumPlayers;
            SessionSettings->NumPrivateConnections = 2;
            SessionSettings->bAllowInvites = true;
            SessionSettings->bAllowJoinInProgress = true;
            SessionSettings->bShouldAdvertise = true;
            SessionSettings->bAllowJoinViaPresence = true;
            SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

            if (!Map.ToString().IsEmpty())
            {
                SessionInfo.GameMapName = Map;

                SessionSettings->Set(SERVER_NAME_SETTINGS_KEY, SessionInfo.GameMapName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);
            }

            OnCreateSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

            //Delegate should get called when this is complete
            return OnlineSessionInterface->CreateSession(*UserId, SESSION_NAME, *SessionSettings);
        }
        else
        {
            return false;
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsytem found!"));

        return false;
    }
}

/// <summary>
/// Function fired when a session create request has completed.
/// </summary>
/// <param name="SessionName">Name of the session this callback is for.</param>
/// <param name="bWasSuccessful">True if the async action completed without error, false if there was an error.</param>
void USTK_GameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

    const IOnlineSubsystem* OnlineSubsystemInterface = IOnlineSubsystem::Get();

    if (OnlineSubsystemInterface)
    {
        IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystemInterface->GetSessionInterface();

        if (OnlineSessionInterface.IsValid())
        {
            OnlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);

            if (bWasSuccessful)
            {
                OnStartSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
                OnlineSessionInterface->StartSession(SESSION_NAME/*SessionInfo.SessionName*/);
            }
        }
    }
}

/// <summary>
/// Function fired when a session start request has completed.
/// </summary>
/// <param name="SessionName">Name of the session this callback is for.</param>
/// <param name="bWasSuccessful">True if the async action completed without error, false if there was an error.</param>
void USTK_GameInstance::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

    const IOnlineSubsystem* OnlineSubsystemInterface = IOnlineSubsystem::Get();

    if (OnlineSubsystemInterface)
    {
        IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystemInterface->GetSessionInterface();

        if (OnlineSessionInterface.IsValid())
        {
            OnlineSessionInterface->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
        }
    }

    if (bWasSuccessful)
    {
        if (!SessionInfo.GameMapName.ToString().IsEmpty())
        {
            FString URL = "/Game/Maps/MainMap_Hospital/MapTest.MapTest?listen";
            GetWorld()->ServerTravel(URL, true);
        }

        IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystemInterface->GetSessionInterface();
        const ULocalPlayer* LocalPlayer = GetFirstGamePlayer();
        IOnlineFriendsPtr FriendInterface = Online::GetFriendsInterface();

        FriendInterface->ReadFriendsList(LocalPlayer->GetControllerId(), EFriendsLists::ToString(EFriendsLists::Default), OnReadFriendsListCompleteDelegate);
    }
}

bool USTK_GameInstance::JoinSession()
{
    const ULocalPlayer* LocalPlayer = GetFirstGamePlayer();

    if (LocalPlayer->IsValidLowLevelFast())
    {
        FOnlineSessionSearchResult SearchResult;

        if (SessionSearch->SearchResults.Num() > 0)
        {
            for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
            {
                if (SessionSearch->SearchResults[i].Session.OwningUserId != LocalPlayer->GetPreferredUniqueNetId())
                {
                    SearchResult = SessionSearch->SearchResults[i];

                    FUniqueNetIdWrapper UniqueNetIdWrapper = FUniqueNetIdWrapper(LocalPlayer->GetPreferredUniqueNetId());

                    //Join the session
                    return JoinSession(UniqueNetIdWrapper.GetUniqueNetId(), SESSION_NAME/*GameSessionName*/, SearchResult);
                }
            }
        }
    }

    return false;
}

/// <summary>
/// Joins a session via a search result.
/// </summary>
/// <param name="UserId">User that started the request.</param>
/// <param name="SessionName">Name of the session.</param>
/// <param name="SearchResult">Session to join.</param>
/// <returns>True if successful, false otherwise.</returns>
bool USTK_GameInstance::JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName,
    const FOnlineSessionSearchResult& SearchResult)
{
    IOnlineSubsystem* OnlineSubsystemInterface = IOnlineSubsystem::Get();

    if (OnlineSubsystemInterface)
    {
        IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystemInterface->GetSessionInterface();

        if (OnlineSessionInterface.IsValid() && UserId.IsValid())
        {
            return OnlineSessionInterface->JoinSession(*UserId, SESSION_NAME, SearchResult);
        }
    }

    return false;
}

/// <summary>
/// Delegate fired when a session join request has completed.
/// </summary>
/// <param name="SessionName">Name of the session this callback is for.</param>
/// <param name="Result">True if the async action completed without error, false if there was an error.</param>
void USTK_GameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>(Result)));

    const IOnlineSubsystem* OnlineSubsystemInterface = IOnlineSubsystem::Get();

    if (OnlineSubsystemInterface)
    {
        IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystemInterface->GetSessionInterface();

        if (OnlineSessionInterface.IsValid())
        {
            OnlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

            APlayerController* PlayerController = GetFirstLocalPlayerController();
            FString TravelURL;

            if (PlayerController && OnlineSessionInterface->GetResolvedConnectString(SESSION_NAME, TravelURL))
            {
                PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
            }
        }
    }
}

bool USTK_GameInstance::FindSessions(bool bIsLAN, bool bIsPresence)
{
    const ULocalPlayer* LocalPlayer = GetFirstGamePlayer();

    if (LocalPlayer->IsValidLowLevelFast())
    {
        FUniqueNetIdWrapper UniqueNetIdWrapper = FUniqueNetIdWrapper(LocalPlayer->GetPreferredUniqueNetId());

        //Find sessions
        FindSessions(UniqueNetIdWrapper.GetUniqueNetId(), bIsLAN, bIsPresence);

        return true;
    }

    return false;
}

/// <summary>
/// Finds an online session.
/// </summary>
/// <param name="UserId">User that initiated the request.</param>
/// <param name="bIsLAN">Are we searching for LAN matches?</param>
/// <param name="bIsPresence">Are we searching for presence sessions?</param>
void USTK_GameInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence)
{
    const IOnlineSubsystem* OnlineSubsystemInterface = IOnlineSubsystem::Get();

    if (OnlineSubsystemInterface)
    {
        IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystemInterface->GetSessionInterface();

        if (OnlineSessionInterface.IsValid() && UserId.IsValid())
        {
            //Set session search settings
            SessionSearch = MakeShareable(new FOnlineSessionSearch());

            SessionSearch->bIsLanQuery = bIsLAN;
            SessionSearch->MaxSearchResults = 20;
            SessionSearch->PingBucketSize = 50;

            if (bIsPresence)
            {
                SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
            }

            TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

            OnFindSessionsCompleteDelegateHandle = OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
            OnlineSessionInterface->FindSessions(*UserId, SearchSettingsRef);
        }
    }
}

/// <summary>
/// Delegate fired when a session search query has completed.
/// </summary>
/// <param name="bWasSuccessful">true if the async action completed without error, false if there was an error.</param>
void USTK_GameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OFindSessionsComplete bSuccess: %d"), bWasSuccessful));

    const IOnlineSubsystem* OnlineSubsystemInterface = IOnlineSubsystem::Get();

    if (OnlineSubsystemInterface)
    {
        IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystemInterface->GetSessionInterface();

        if (OnlineSessionInterface.IsValid())
        {
            OnlineSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num()));

            if (SessionSearch->SearchResults.Num() > 0)
            {
                for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Session Number: %d | Sessionname: %s "), SearchIdx + 1, *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName)));
                }
            }
        }
    }
}

/// <summary>
/// Delegate used when reading friends list using query.
/// </summary>
/// <param name="LocalUserNum">The controller number of the associated user that made the request.</param>
/// <param name="bWasSuccessful">True if the async action completed without error, false if there was an error.</param>
/// <param name="FriendsListName">Name of the friends list that was operated on.</param>
/// <param name="ErrorString">String representing the error condition.</param>
void USTK_GameInstance::OnReadFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& FriendsListName,
    const FString& ErrorString)
{
    if (bWasSuccessful)
    {
        IOnlineFriendsPtr FriendInterface = Online::GetFriendsInterface();

        if (FriendInterface.IsValid())
        {
            FriendsList.Empty();
            FriendArray.Empty();

            FriendInterface->GetFriendsList(LocalUserNum, FriendsListName, FriendsList);

            if (FriendsList.Num() > 0)
            {
                const IOnlineSubsystem* OnlineSubsystemInterface = IOnlineSubsystem::Get();

                if (OnlineSubsystemInterface)
                {
                    IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystemInterface->GetSessionInterface();

                    //Grab friend data and add it to the friend array
                    for (size_t i = 0; i < FriendsList.Num(); i++)
                    {
                        FFriendData FriendData = FFriendData();

                        FriendData.DisplayName = FriendsList[i].Get().GetDisplayName();
                        FriendData.Presence = EOnlinePresenceState::ToString(FriendsList[i].Get().GetPresence().Status.State);
                        FriendData.RealName = FriendsList[i].Get().GetRealName();
                        FriendData.UniqueNetId = FriendsList[i].Get().GetUserId().Get().ToString();

                        FriendArray.Add(FriendData);
                    }
                }
            }
            else
            {
                GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("You have no friends :("));
            }
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Failed to read friends: %s"), *ErrorString));
    }
}

void USTK_GameInstance::InviteFriend(FString FriendName, bool bWasSuccessful)
{
    const IOnlineSubsystem* OnlineSubsystemInterface = IOnlineSubsystem::Get();

    if (OnlineSubsystemInterface)
    {
        IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystemInterface->GetSessionInterface();

        for (size_t i = 0; i < FriendsList.Num(); i++)
        {
            if (FriendsList[i].Get().GetRealName().Contains(FriendName))
            {
                FUniqueNetIdWrapper UniqueNetIdWrapper = FUniqueNetIdWrapper(FriendsList[i].Get().GetUserId());
                const ULocalPlayer* LocalPlayer = GetFirstGamePlayer();

                if (OnlineSessionInterface->SendSessionInviteToFriend(LocalPlayer->GetControllerId(), SESSION_NAME/*SessionInfo.SessionName*/, *UniqueNetIdWrapper.GetUniqueNetId()))
                {
                    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Purple, FString::Printf(TEXT("Invited friend %d"), bWasSuccessful));
                }
            }
        }
    }
}

bool USTK_GameInstance::SendSessionInviteToFriend(const FString& FriendUniqueNetId)
{
    const IOnlineSubsystem* OnlineSubsystemInterface = IOnlineSubsystem::Get();

    if (OnlineSubsystemInterface)
    {
        IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystemInterface->GetSessionInterface();

        for (size_t i = 0; i < FriendsList.Num(); i++)
        {
            if (FriendsList[i].Get().GetUserId()->ToString().Contains(FriendUniqueNetId))
            {
                FUniqueNetIdWrapper UniqueNetIdWrapper = FUniqueNetIdWrapper(FriendsList[i].Get().GetUserId());
                const ULocalPlayer* LocalPlayer = GetFirstGamePlayer();

                if (OnlineSessionInterface->SendSessionInviteToFriend(LocalPlayer->GetControllerId(), SESSION_NAME/*SessionInfo.SessionName*/, *UniqueNetIdWrapper.GetUniqueNetId()))
                {
                    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Purple, FString::Printf(TEXT("Invited friend")));
                }
            }
        }
    }

    return false;
}

/// <summary>
/// Called when a user accepts a session invitation. Allows the game code a chance to
/// clean up any existing state before accepting the invite. The invite must be accepted by
/// calling JoinSession() after clean up has completed.
/// </summary>
/// <param name="bWasSuccesful">True if the async action completed without error, false if there was an error.</param>
/// <param name="LocalUserNum">The controller number of the accepting user.</param>
/// <param name="NetId">The user being invited.</param>
/// <param name="SessionSearchResult">The search/settings for the session we're joining via invite.</param>
void USTK_GameInstance::OnSessionUserInviteAccepted(const bool bWasSuccesful, const int32 LocalUserNum, TSharedPtr<const FUniqueNetId> NetId,
    const FOnlineSessionSearchResult& SessionSearchResult)
{
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnSessionUserInviteAccepted: %d"), bWasSuccesful));

    if (bWasSuccesful)
    {
        if (SessionSearchResult.IsValid())
        {
            IOnlineSessionPtr SessionInt = IOnlineSubsystem::Get()->GetSessionInterface();
            SessionInt->JoinSession(LocalUserNum, SESSION_NAME/*GameSessionName*/, SessionSearchResult);
        }
    }
}

void USTK_GameInstance::DestroySession()
{
    const IOnlineSubsystem* OnlineSubsystemInterface = IOnlineSubsystem::Get();

    if (OnlineSubsystemInterface)
    {
        IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystemInterface->GetSessionInterface();

        if (OnlineSessionInterface.IsValid())
        {
            OnlineSessionInterface->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
            OnlineSessionInterface->DestroySession(SESSION_NAME);
        }
    }
}

/// <summary>
/// Delegate fired when a destroying an online session has completed.
/// </summary>
/// <param name="SessionName">Name of the session this callback is for.</param>
/// <param name="bWasSuccessful">True if the async action completed without error, false if there was an error.</param>
void USTK_GameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnDestroySessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

    IOnlineSubsystem* OnlineSubsystemInterface = IOnlineSubsystem::Get();

    if (OnlineSubsystemInterface)
    {
        IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystemInterface->GetSessionInterface();

        if (OnlineSessionInterface.IsValid())
        {
            OnlineSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

            if (bWasSuccessful)
            {
                UGameplayStatics::OpenLevel(GetWorld(), SessionInfo.EntryMapName, true);
            }
        }
    }
}

/// <summary>
/// Creates and sets up the Main Menu Widget in the game's viewport.
/// </summary>
void USTK_GameInstance::SetupMainMenuWidget()
{
    if (!ensure(MainMenuClass != nullptr)) return;

    UWMainMenu = CreateWidget<USTK_UWMainMenu>(this, MainMenuClass);
    if (!ensure(UWMainMenu != nullptr)) return;

    UWMainMenu->Setup();
}

/// <summary>
/// Creates and sets up the Pause Menu Widget in the game's viewport.
/// </summary>
void USTK_GameInstance::SetupPauseMenuWidget()
{
    if (!ensure(PauseMenuClass != nullptr)) return;

    UWPauseMenu = CreateWidget<USTK_UWPauseMenu>(this, PauseMenuClass);
    if (!ensure(UWPauseMenu != nullptr)) return;

    UWPauseMenu->Setup();
}

void USTK_GameInstance::SetupInviteMenuWidget()
{
    if (!ensure(InviteMenuClass != nullptr)) return;

    UWInviteMenu = CreateWidget<USTK_UWInviteMenu>(GetWorld(), InviteMenuClass);
    if (!ensure(UWInviteMenu != nullptr)) return;

    UWInviteMenu->Setup();
}

/// <summary>
/// Removes the Main Menu Widget from the game and loads the game's default level.
/// </summary>
void USTK_GameInstance::LoadGameLevel()
{
    UWorld* World = GetWorld();
    if (!ensure(World != nullptr)) return;

    if (!ensure(UWMainMenu != nullptr)) return;
    UWMainMenu->Teardown();

    UGameplayStatics::OpenLevel(World, "MapTest");
}







