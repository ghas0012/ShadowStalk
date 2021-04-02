// Fill out your copyright notice in the Description page of Project Settings.

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
