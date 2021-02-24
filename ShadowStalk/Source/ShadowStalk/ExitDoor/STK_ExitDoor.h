// Fill out your copyright notice in the Description page of Project Settings.

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

protected:

	virtual void BeginPlay() override;

	bool bIsOpen = false;

public:

	virtual void Tick(float DeltaTime) override;

	void DoorOpen();
	void DoorClose();

	bool GetIsOpen();

};
