// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STK_EyeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHADOWSTALK_API USTK_EyeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USTK_EyeComponent();

	void SetupMesh(USkeletalMeshComponent* meshptr);
	class USkeletalMeshComponent* TargetMesh = nullptr;

protected:

	struct StateData
	{
		TMap <FName, float> State;		// A list of all the morph targets and their values
		FName activeState;				// The name of the activated state, for ease of access
		float HoldDuration = 0.1f;		// How long do we hold a gesture?
		float TransitionSpeed = 1.0f;	// How quickly do we transition to a gesture?
		float fidget = 0.0f;			// How often/hard do we fidget that state?

		StateData& operator=(const StateData& sd)
		{
			// self-assignment check
			if (this == &sd)
				return *this;

			StateData* to_return = new StateData();
			to_return->State = sd.State;
			to_return->activeState = sd.activeState;
			to_return->HoldDuration = sd.HoldDuration;
			to_return->TransitionSpeed = sd.TransitionSpeed;
			to_return->fidget = sd.fidget;

			return *to_return;
		};
	};

	// Called when the game starts
	virtual void BeginPlay() override;

	void LerpQueue(float DeltaTime);
	void RandomBlinkLogic(float DeltaTime);
	void ApplyFidgeting(float DeltaTime, StateData* StateToFidget);

	TArray<FName> GestureNames = TArray<FName>{ FName("Neutral"), FName("Sad"), FName("Angry"), FName("Happy"), FName("Close") };

	StateData* CurrentState;
	TArray<StateData*> StatesQueue;

	float lerpValue = 0.0f;

	TMap <FName, float> MakeGestureMap(std::string targetEmotion, float percent);
	TMap <FName, float> GetCurrentState();

	float blinkTimer;
	float fidgetTime;
	float fidgetRecordedIntensity = 1.0f;
	float fidgetOriginalIntensity = 1.0f;
	float fidgetLerpSpeed = 2.0f;
	float fidgetLerpFactor = 0.0f;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Reset();

	void Blink(float duration, float speed);
	void BlankFace(float duration, float speed);

	void SetEmotion(std::string Name, float intensity, float duration, float transitionSpeed, float fidget);
	void SetEmotion(std::string Name, float intensity, float duration, float transitionSpeed);
	void SetEmotion(std::string Name, float intensity, float duration);
	void SetEmotion(std::string Name, float intensity);
	void SetEmotion(std::string Name);

	float GetCurrentEyeClosedLevel();
	float GetValueOfSpecificMorphTarget(std::string name);

};
