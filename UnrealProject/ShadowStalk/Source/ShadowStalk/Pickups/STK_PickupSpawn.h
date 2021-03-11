// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Hamidreza Ghasemi

// Changelog:
// - Class init.


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
