// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Hamidreza Ghasemi
  Date Modified: 3/12/2021
  Comment/Description:

	An exit door class. It has a collider, a mesh and some particle effects.
	It can take requests to open and close.

  ChangeLog:
  H 3/12/2021: Initialized class.
  H 3/16/2021: Reformatted code, Added a class description and summaries to relevant methods.
  C 4/07/2021: Added Code to NetMulticast the Door's.
  J 4/13/2021: Added Teleport Sound for when Win State is made.

*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STK_ExitDoor.generated.h"

UCLASS()
class SHADOWSTALK_API ASTK_ExitDoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTK_ExitDoor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
		class USceneComponent* SceneComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
		class UBoxComponent* DoorCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
		class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		class UParticleSystemComponent* ParticleFX;

	//Sound

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door|Audio", meta = (AllowPrivateAccess = "true"))
		class USoundBase* TeleportSound;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bIsOpen = false;

public:

	void DoorOpen();

	UFUNCTION(NetMulticast, Reliable)
		void NMC_DoorOpen();

	void DoorClose();

	UFUNCTION(NetMulticast, Reliable)
		void NMC_DoorClose();

	UFUNCTION(BlueprintCallable)
	bool GetIsOpen();

};
