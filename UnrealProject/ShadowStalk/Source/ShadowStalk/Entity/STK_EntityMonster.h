// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Christian Young, Hamidreza Ghasemi
  Date Modified: 3/12/2021
  Comment/Description: 

	A child of the Entity class.
	It reacts to user input for basic movement. It can attack and execute Shades,

  ChangeLog:
  C 3/12/2021: Created the base entity monster class.
  H 3/12/2021: Added basic attack logic and input locking. Also added states.
  H 3/15/2021: Added entity safety check on attack logic. Fixed attack range and direction. 
  H 3/16/2021: Added a class description and summaries to pertinent functions.
  C 3/19/2021: Added Networking code
  H 3/23/2021: Moved movement data into its own struct.
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

	//Sound

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		class USoundBase* MonsterAttackScream;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		class USoundBase* MonsterStunned;

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

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Interact();

	void ExecuteShade(class ASTK_EntityShade* TargetShade);

	UFUNCTION(Server, Reliable)
	void Server_ExecuteShade(class ASTK_EntityShade* TargetShade);

	void OnExcecuteEnd();

	UFUNCTION(Server, Reliable)
	void Server_OnExcecuteEnd();

	void Attack();

	UFUNCTION(Server, Reliable)
	void Server_Attack();
};
