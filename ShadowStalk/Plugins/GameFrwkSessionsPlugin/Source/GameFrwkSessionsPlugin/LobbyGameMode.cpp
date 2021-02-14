// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"

#include "TimerManager.h"

#include "OnlineGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;

	if (NumberOfPlayers >= MaxNumberOfPlayers)
	{
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, 10);
	}

	
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}

void ALobbyGameMode::StartGame()
{
	auto GameInstance = Cast<UOnlineGameInstance>(GetGameInstance());

	if (GameInstance == nullptr) return;

	//GameInstance->MapToPlay = "TestMap";
	
	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	//World'/GameFrwkSessionsPlugin/Maps/Game.Game'  --- World'/Game/Levels/Sandbox/LV_Sandbox.LV_Sandbox'
	
	//World->ServerTravel("/Game/Levels/Sandbox/LV_Sandbox?listen");

    //World->ServerTravel("/Game/Levels/Sandbox/TestMap?listen");
	//World->ServerTravel("/Game/Levels/Sandbox/" + GameInstance->MapToPlay + "?listen");
	//void travel(int32 selectedMap, int32 selectedGameMode, FString additionalOptions) {
		FString URL = GameInstance->MapToPlay->MapPath;// MapNames[selectedMap];
		URL += "?Game=" + GameInstance->MapToPlay->GameModePath;//GameInstance->GameModes[0]->GetPathName();//GetName();/*->GetClass()->ClassDefaultObject->GetClass()->GetName()*/; //->GetFullName(nullptr, EObjectFullNameFlags::None);// GetDisplayNameText().ToString() ;
		URL += "?listen";

		///bUseSeamlessTravel = true;
		GetWorld()->ServerTravel(URL);
	//}

//	World->ServerTravel(GameInstance->MapToPlay + "?listen");
    //World->SeamlessTravel("/Game/Levels/Sandbox/TestMap?listen", true);
}

/*
UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Names")
TArray<FString> MapNames;

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Modes")
TArray<TSubclassOf<class AGameMode>> GameModes;

void Travel(int32 selectedMap, int32 selectedGameMode, FString additionalOptions){
   FString URL = MapNames[selectedMap];
   URL += "?Game=" + GameModes[selectedGameMode];
   URL += "?listen";

   GetWorld()->ServerTravel(URL);
}
*/