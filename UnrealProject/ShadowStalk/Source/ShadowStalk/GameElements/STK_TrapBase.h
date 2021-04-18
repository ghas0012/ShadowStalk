// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Jeffrey Armstrong
  Date Modified: 3/28/2021
  Comment/Description:

	A Trap Class for when a Shade walks over it. It has a collider, a mesh, a scene root component, and variables for activating and disabling the trap.

  ChangeLog:
  J 3/28/2021: Initialized class.
  J 4/14/2021: Added a class description and summaries to relevant methods.
  J 4/15/2021: Added Trap Activate sound.

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trap|Audio", meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trap|Audio", meta = (AllowPrivateAccess = "true"))
		class USoundBase* TrapSound;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
