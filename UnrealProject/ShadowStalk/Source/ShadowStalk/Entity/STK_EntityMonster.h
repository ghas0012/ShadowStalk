// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Christian Young, Hamidreza Ghasemi
  Date Modified: 3/12/2021
  Comment/Description: *Description of Class*
  ChangeLog:
  C 3/12/2021: Created the base entity monster class.
  H 3/12/2021: Added basic attack logic and input locking.
*/

#pragma once

#include "CoreMinimal.h"
#include "STK_Entity.h"
#include "STK_EntityMonster.generated.h"

UENUM(BlueprintType)
enum class EMonsterState : uint8 {
	Default		UMETA(DisplayName = "Default"),
	Executing	UMETA(DisplayName = "Executing"),
	Stunned		UMETA(DisplayName = "Stunned")
};

UCLASS()
class SHADOWSTALK_API ASTK_EntityMonster : public ASTK_Entity
{
	GENERATED_BODY()


public:

	ASTK_EntityMonster();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float GrabRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackKnockbackStrength = 80000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float ExecutionTimeLength = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float ExecutionPositioningDistance = 250;

	FTimerHandle ExecutionTimerHandle;

protected:

	EMonsterState CurrentState = EMonsterState::Default;

	class ASTK_MatchGameState* gamestate = nullptr;

	virtual void BeginPlay() override;


public:

	virtual EEntityType GetEntityType() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	EMonsterState GetMonsterState();

	UFUNCTION(BlueprintCallable)
	void SetMonsterState(EMonsterState state);

	virtual void Interact() override;

	void ExecuteShade(class ASTK_EntityShade* TargetShade);

	void OnExcecuteEnd();

	void Attack();
};