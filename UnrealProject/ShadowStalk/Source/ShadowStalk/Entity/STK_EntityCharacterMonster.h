// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Christian Young, Hamidreza Ghasemi
  Date Modified: 3/12/2021
  Comment/Description: 

	A child of the EntityCharacter class.
	It reacts to user input for basic movement. It can attack and execute Shades,

  ChangeLog:
  C 3/12/2021: Created the base EntityCharacter monster class.
  H 3/12/2021: Added basic attack logic and input locking. Also added states.
  H 3/15/2021: Added EntityCharacter safety check on attack logic. Fixed attack range and direction. 
  H 3/16/2021: Added a class description and summaries to pertinent functions.
  C 3/19/2021: Added Networking code
  H 3/23/2021: Moved movement data into its own struct.
*/

#pragma once

#include "CoreMinimal.h"
#include "STK_EntityCharacter.h"
#include "STK_EntityCharacterMonster.generated.h"

UENUM(BlueprintType)
enum class ECharacterMonsterState : uint8 {
	Default		UMETA(DisplayName = "Default"),
	Executing	UMETA(DisplayName = "Executing"),
	Stunned		UMETA(DisplayName = "Stunned")
};

UCLASS()
class SHADOWSTALK_API ASTK_EntityCharacterMonster : public ASTK_EntityCharacter
{
	GENERATED_BODY()

public:

	ASTK_EntityCharacterMonster();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Attack")
	float GrabRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Attack")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Attack")
	float AttackKnockbackStrength = 80000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Attack")
	float ExecutionTimeLength = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Attack")
	float ExecutionPositioningDistance = 250;

	FTimerHandle ExecutionTimerHandle;

	//Sound

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster|Audio", meta = (AllowPrivateAccess = "true"))
		class USoundBase* MonsterAttackSound;

protected:

	ECharacterMonsterState CurrentState = ECharacterMonsterState::Default;

	class ASTK_MatchGameState* gamestate = nullptr;

	virtual void BeginPlay() override;


public:

	virtual EEntityType GetEntityType() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	ECharacterMonsterState GetMonsterState();

	UFUNCTION(BlueprintCallable)
	void SetMonsterState(ECharacterMonsterState state);

	virtual void Interact() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Interact();

	void ExecuteShade(class ASTK_EntityCharacterShade* TargetShade);

	UFUNCTION(Server, Reliable)
	void Server_ExecuteShade(class ASTK_EntityCharacterShade* TargetShade);

	void OnExcecuteEnd();

	UFUNCTION(Server, Reliable)
	void Server_OnExcecuteEnd();

	void Attack();

	UFUNCTION(Server, Reliable)
	void Server_Attack();
};
