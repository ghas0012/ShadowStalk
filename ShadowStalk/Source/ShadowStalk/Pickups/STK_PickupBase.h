// Fill out your copyright notice in the Description page of Project Settings.

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
		float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		bool bAnimate = true;							   
														   
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		bool bBob = true;								   
														   
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		bool bRotate = true;							
														
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		float bobFreq = 1.0f;							
														
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		float bobAmp = 1.0f;						
													
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		float rotFreq = 1.0f;						
													
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		FRotator RotDir;

	UFUNCTION()
		virtual void Enable();

	UFUNCTION()
		virtual void Disable();

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
