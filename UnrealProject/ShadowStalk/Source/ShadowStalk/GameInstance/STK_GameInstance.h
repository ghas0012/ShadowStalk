// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

 /*
  Author: Arianne Fennell
  Date Modified: 3/23/2021
  Comment/Description: Base Game Instance for the game.
  ChangeLog:
  A 3/12/2021: Implemented base functions to work with UWMainMenu.
  A 3/13/2021: Added SetupCreditWidget() to work with UWCreditsPanel.
  A 3/23/2021: Moved Options and Credits setup to UWMainMenu. Added Pause Menu Setup.
  A 3/29/2021: Implemented key functions for Join/Host Online Sessions.
  */

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "Online.h"
#include "OnlineSubsystemUtils.h"
#include "Engine/GameInstance.h"
#include "ShadowStalk/Interfaces/STK_MenuInterface.h"
#include "STK_GameInstance.generated.h"

USTRUCT(BlueprintType)
struct FOnlineGameSessionInfo
{
    GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadWrite, Category = "Online|Session")
    FName SessionName;

    UPROPERTY(BlueprintReadWrite, Category = "Online|Session")
    FName GameMapName;

    UPROPERTY(BlueprintReadWrite, Category = "Online|Session")
    FName EntryMapName;
};


USTRUCT(BlueprintType)
struct FFriendData
{
    GENERATED_USTRUCT_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Friend")
    FString DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Friend")
    FString RealName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Friend")
    FString Presence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Friend")
    FString UniqueNetId;
};

UCLASS()
class SHADOWSTALK_API USTK_GameInstance : public UGameInstance
{
    GENERATED_BODY()

public:

    USTK_GameInstance(const FObjectInitializer& ObjectInitializer);

    virtual void Init() override;

    virtual void Shutdown() override;

    UFUNCTION(BlueprintCallable, Category = "Online|Session")
    bool HostSession(FName SessionName, FName Map, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);

    UFUNCTION(BlueprintCallable, Category = "Online|Session")
    bool FindSessions(bool bIsLAN, bool bIsPresence);

    UFUNCTION(BlueprintCallable, Category = "Online|Session")
    bool JoinSession();

    UFUNCTION(BlueprintCallable, Category = "Online|Session")
    void DestroySession();

    UFUNCTION(BlueprintCallable, Category = "Online|Session")
    bool SendSessionInviteToFriend(const FString& FriendUniqueNetId);

    void InviteFriend(FString FriendName, bool bWasSuccessful);

    bool CreateSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, FName Map, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);

    bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

    void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);

    TArray<FFriendData> GetFriends() { return FriendArray; }

    UFUNCTION(BlueprintCallable)
    void SetupMainMenuWidget();

    UFUNCTION(BlueprintCallable)
    void SetupPauseMenuWidget();

    UFUNCTION(BlueprintCallable)
    void LoadGameLevel();

private:

    TArray<FFriendData> FriendArray;

    TSharedPtr<class FOnlineSessionSettings> SessionSettings;

    TSharedPtr<class FOnlineSessionSearch> SessionSearch;

    TArray<TSharedRef<FOnlineFriend>> FriendsList;

    FOnlineGameSessionInfo SessionInfo;

    //Delegate called when session created
    FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;

    //Delegate called when session started 
    FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

    //Delegate for searching for sessions
    FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

    //Delegate for joining a session
    FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

    //Delegate for destroying a session
    FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

    //Delegate for reading friends list using query
    FOnReadFriendsListComplete OnReadFriendsListCompleteDelegate;

    //Delegate for when an invite is accepted (including rich presence)
    FOnSessionUserInviteAcceptedDelegate OnSessionUserInviteAcceptedDelegate;

    //Handles to registered delegates for creation
    FDelegateHandle OnCreateSessionCompleteDelegateHandle;

    //Handles to registered delegates for starting
    FDelegateHandle OnStartSessionCompleteDelegateHandle;

    //Handle to registered delegate for searching a session
    FDelegateHandle OnFindSessionsCompleteDelegateHandle;

    //Handle to registered delegate for joining a session
    FDelegateHandle OnJoinSessionCompleteDelegateHandle;

    //Handle to registered delegate for destroying a session
    FDelegateHandle OnDestroySessionCompleteDelegateHandle;

    //Handles to registered delegates for accepting an invite
    FDelegateHandle OnSessionUserInviteAcceptedDelegateHandle;

    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

    void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

    void OnFindSessionsComplete(bool bWasSuccessful);

    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

    void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

    void OnReadFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& FriendsListName, const FString& ErrorString);

    void OnSessionUserInviteAccepted(const bool bWasSuccesful, const int32 LocalUserNum, TSharedPtr<const FUniqueNetId> NetId, const FOnlineSessionSearchResult& SessionSearchResult);

protected:

    TSubclassOf<class UUserWidget> MainMenuClass;
    class USTK_UWMainMenu* UWMainMenu;

    TSubclassOf<class UUserWidget> PauseMenuClass;
    class USTK_UWPauseMenu* UWPauseMenu;
};
