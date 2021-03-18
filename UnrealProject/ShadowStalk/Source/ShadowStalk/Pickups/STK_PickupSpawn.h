// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Hamidreza Ghasemi
  Date Modified: 3/12/2021
  Comment/Description:	

	  This class is an interface for level designers to place item and key spawn points around the map with ease.
	  It has an enum identifier for what pickup object it’s intended to spawn.
	  On the server, this class registers itself with the Match GameMode Module, which decides which spawn points to randomly spawn keys on.
	  A billboard sprite updated through code will change its icon in the editor for more readability.

  ChangeLog:
  H 3/12/2021: class init.
  H 3/15/2021: Added Super::BeginPlay() to resolve a bug.
  H 3/16/2021: Added a class description and summaries to relevant methods.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShadowStalk/ShadowStalk.h"
#include "STK_PickupSpawn.generated.h"

class UTexture2D;

UCLASS()
class SHADOWSTALK_API ASTK_PickupSpawn : public AActor
{
	GENERATED_BODY()


public:	
	// Sets default values for this actor's properties
	ASTK_PickupSpawn();

	class USceneComponent* SceneComp = nullptr;
	class UBillboardComponent* EditorSpriteComponent = nullptr;

	UTexture2D* Sprite_Key = nullptr;
	UTexture2D* Sprite_Item = nullptr;
	UTexture2D* Sprite_Undefined = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSpawn")
	EPickupType SpawnType;


protected:

	virtual void PostLoad() override;
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:	
	
	EPickupType GetPickupType();
};
