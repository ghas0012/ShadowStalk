// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Christian Young

// Changelog:
// - Class init.

#pragma once

#include "CoreMinimal.h"
#include "STK_Entity.h"
#include "STK_EntityMonster.generated.h"

UENUM(BlueprintType)
enum class EMonsterState : uint8 {
	Default  UMETA(DisplayName = "Default"),
	Stunned  UMETA(DisplayName = "Stunned")
};

UCLASS()
class SHADOWSTALK_API ASTK_EntityMonster : public ASTK_Entity
{
	GENERATED_BODY()


public:

	ASTK_EntityMonster();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		class UAnimationAsset* AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		class UAnimationAsset* KillAnimation;

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


	void Attack();
};
