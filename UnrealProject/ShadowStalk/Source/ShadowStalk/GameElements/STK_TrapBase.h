// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Jeffrey Armstrong
  Date Modified: 3/28/2021
  Comment/Description:

	Base class for the Bear Trap in the game.
	It has a Collider, a Mesh, and variables for deactivating the collision.

  ChangeLog:
  J 3/28/2021: Class init.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STK_TrapBase.generated.h"

UCLASS()
class SHADOWSTALK_API ASTK_TrapBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTK_TrapBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap")
		class USceneComponent* SceneRootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		class UStaticMeshComponent* TrapMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap")
		class UBoxComponent* TrapCollider;

	virtual void ActivateTrap();

	virtual void DisableTrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
