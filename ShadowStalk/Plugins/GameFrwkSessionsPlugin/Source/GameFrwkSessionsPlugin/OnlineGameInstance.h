// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFrwkSessionsPlugin.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineFriendsInterface.h"
//#include "Net/OnlineBlueprintCallProxyBase.h"
#include "Engine.h"

#include "OnlineSubsystemUtils.h"

#include "MenuSystem/MenuInterface.h"
#include "OnlineGameInstance.generated.h"

USTRUCT()
struct FMapData
{
	GENERATED_BODY()

	UPROPERTY()
	FString MapName;

	UPROPERTY()
	FString MapPath;

	UPROPERTY()
	FString GameMode;

	UPROPERTY()
	FString GameModePath;

};


USTRUCT()
struct FGameModeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AGameModeBase> GameMode;

	UPROPERTY(EditAnywhere)
		FString GameModeName;

	//UPROPERTY()
	//	FString GameModePath;

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
/**
 * 
 */
UCLASS()
class GAMEFRWKSESSIONSPLUGIN_API UOnlineGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UOnlineGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void LoadMenuWidget();

	UFUNCTION(BlueprintCallable)
	void InGameLoadMenu();

	UFUNCTION(Exec)
	virtual void Host(FString ServerName, FString Team, FString Map) override;

	UFUNCTION(Exec)
	virtual void Join(uint32 Index, FString Team) override;

	void StartSession();

	virtual void LoadMainMenu() override;

	void RefreshServerList() override;
    
    FString DesiredTeamNum;

	TSharedPtr<FMapData> MapToPlay;

	//Game Modes Can be Set In The BP
	UPROPERTY(EditAnywhere, Category = "Game Modes")
		TArray<FGameModeData> GameModes;


	FORCEINLINE TArray<TSharedPtr<FMapData>> GetMapList(){ return MapList;} 

	void SetMapToPlay(FString map);

	
	void SetGameModeToPlay(FString SelectedGameMode);


	UPROPERTY(EditDefaultsOnly, Category = "Player Characters")
		TSubclassOf<class ACharacter> Character1; //0

	UPROPERTY(EditDefaultsOnly, Category = "Player Characters")
		TSubclassOf<class ACharacter> Character2; //1

	UPROPERTY(EditDefaultsOnly, Category = "Player Characters")
		int CharacterChoice = 0;

	
	void OnReadFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);
	void OnSessionUserInviteAccepted(const bool bWasSuccesful, const int32 LocalUserNum, TSharedPtr<const FUniqueNetId> NetId, const FOnlineSessionSearchResult& SessionSearchResult);
	
	TArray< TSharedRef<FOnlineFriend> > FriendsList;
	TArray<FFriendData>FriendsArr;
	FOnReadFriendsListComplete  OnReadFriendsListCompleteDelegate;
	FOnSessionUserInviteAcceptedDelegate OnSessionUserInviteAcceptedDelegate;

	void RetrieveFriendsList();
	bool InviteFriend(FString Name);

	IConsoleCommand* InviteFriendsConsoleCommand;
	
private:
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

   TArray<TSharedPtr<FMapData>> MapList;

    //Internal callback when session creation completes, calls StartSession
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FString DesiredServerName;
	FOnlineSessionSettings SessionSettings;
	void CreateSession();

	void ParseMaps();
	void SetGameModeToPlay(TSubclassOf<class AGameModeBase> gm);
};
