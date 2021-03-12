// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Hamidreza Ghasemi
  Date Modified: 3/12/2021
  Comment/Description: *Description of Class*
  ChangeLog:
  H 3/12/2021: *Description of Change*
*/

#pragma once

#include "CoreMinimal.h"

// For EPickupType enums
#include "ShadowStalk/ShadowStalk.h"

#include "GameFramework/Actor.h"
#include "STK_PickupBase.generated.h"


UCLASS()
class SHADOWSTALK_API ASTK_PickupBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTK_PickupBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Base")
		class USceneComponent* SceneRootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		class UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		class UParticleSystemComponent* ParticleFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Base")
		class USphereComponent* Collider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Base")
		float Radius = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		bool bAnimate = true;							   
														   
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		bool bBob = true;								   
														   
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		bool bRotate = true;							
														
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		float bobFreq = 0.1f;							
														
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		float bobAmp = 5.0f;						
													
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		float rotFreq = 0.05f;						
													
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		FRotator RotDir = FRotator(0,1,0);

	UFUNCTION()
		virtual void Disable();

	UFUNCTION()
		virtual void Enable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	EPickupType PickupType;

	float BobAnimVal = 0;
	float RotAnimVal = 0;

	FVector Origin;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual EPickupType GetPickupType();

};
