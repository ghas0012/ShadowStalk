// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved

#include "STK_EyeComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
USTK_EyeComponent::USTK_EyeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// StateData is a Structure to hold all the data necessary to do a gesture transition
	CurrentState = new StateData();
	CurrentState->HoldDuration = 0.0f;
	CurrentState->TransitionSpeed = 1.0f;
	CurrentState->State = MakeGestureMap("Neutral", 1.0f);
	CurrentState->ActiveState = FName("Neutral");
	//StatesQueue.Add(CurrentState);
}


/// <summary>
/// Called when the game starts. It registers the mesh to be modified.
/// </summary>
void USTK_EyeComponent::SetupTPMesh(USkeletalMeshComponent* meshptr)
{
	ThirdPersonMesh = meshptr;
}

/// <summary>
/// Called when the game starts. It registers the eyesocket to be modified.
/// </summary>
void USTK_EyeComponent::SetupFPMesh(USkeletalMeshComponent* eyeSocketMeshPtr)
{
	FirstPersonEyeSocket = eyeSocketMeshPtr;
}

/// <summary>
/// Called when the game starts
/// </summary>
void USTK_EyeComponent::BeginPlay()
{
	Super::BeginPlay();
}


/// <summary>
/// Called every frame. Calls LerpQueue which handles the queue of emotions to process.
/// </summary>
void USTK_EyeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//RandomBlinkLogic(DeltaTime);

	//	TO DEBUG GESTURE STATE:
	//	for (size_t i = 0; i < StatesQueue.Num(); i++)
	//	{
	//		FString x = "";
	//		for (size_t j = 0; j < GestureNames.Num(); j++)
	//		{
	//			if (StatesQueue[i]->State[GestureNames[j]] > 0)
	//			{
	//				x.Append((GestureNames[j].ToString()));
	//			}
	//		}
	//		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, x);
	//	}

	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::Printf(TEXT("Happy: %f \n Blink: %f"), GetValueOfSpecificMorphTarget("Happy"), GetValueOfSpecificMorphTarget("Close")));
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::Printf(TEXT("Neutral: %f \n Sad: %f"), GetValueOfSpecificMorphTarget("Neutral"), GetValueOfSpecificMorphTarget("Sad")));
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::Printf(TEXT("Angry: %f"), GetValueOfSpecificMorphTarget("Angry")));

	//float x;
	//if (StatesQueue.Num() > 0)
	//	x = StatesQueue[0]->Fidget;
	//else
	//	x = CurrentState->Fidget;

	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, FString::Printf(TEXT("Angry: %f"), x));

	if (ThirdPersonMesh == nullptr || ThirdPersonMesh->SkeletalMesh == nullptr || FirstPersonEyeSocket == nullptr || FirstPersonEyeSocket->SkeletalMesh == nullptr)
		return;

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	LerpQueue(DeltaTime);
}



/// <summary>
/// generate a random number every x amount of time.
/// based on that number, Call blink within a range of speed and duration.
/// </summary>
void USTK_EyeComponent::RandomBlinkLogic(float DeltaTime)
{
	blinkTimer += DeltaTime;

	if (blinkTimer > 0.25f )
	{
		blinkTimer = 0;

		if (FMath::RandRange(0, 512) < 45)
		{
			Blink(
				FMath::RandRange(0.0f, 0.1f),
				FMath::RandRange(5.0f, 7.5f)
				);
		}
	}
}


/// <summary>
/// Resets the face and removes all gestures immediately.
/// </summary>
void USTK_EyeComponent::Reset()
{
	for (size_t i = 0; i < GestureNames.Num(); i++)
	{
		ThirdPersonMesh->SetMorphTarget(FName(GestureNames[i]), 0, false);
	}

	CurrentState->HoldDuration = 0;
	CurrentState->TransitionSpeed = 1.0;
	CurrentState->State = MakeGestureMap("Neutral", 1.0f);

	StatesQueue.Empty();
	StatesQueue.Add(CurrentState);
}


/// <summary>
/// Returns a map of gestures mapped to values, based on what gesture is requested, and how intense it is.
/// </summary>
TMap <FName, float> USTK_EyeComponent::MakeGestureMap(std::string targetGesture, float percent)
{
	TMap <FName, float> TargetEyeStates;

	for (size_t i = 0; i < GestureNames.Num(); i++)
	{
		TargetEyeStates.Add(TPair<FName, float>(GestureNames[i], 0.0f));
	}

	if (targetGesture == "Neutral")
	{
		return TargetEyeStates;
	}

	TargetEyeStates[FName(targetGesture.c_str())] = percent;
	return TargetEyeStates;
}


/// <summary>
/// Reads the current state of the mesh and returns a map of gesture values.
/// </summary>
TMap <FName, float> USTK_EyeComponent::GetCurrentState()
{
	TMap <FName, float> state;

	for (size_t i = 0; i < GestureNames.Num(); i++)
	{
		state.Add(TPair<FName, float>(GestureNames[i], ThirdPersonMesh->GetMorphTarget(GestureNames[i])));
	}

	return state;
}

/// <summary>
/// Function to process the queue of gestures to display.
/// </summary>
void USTK_EyeComponent::LerpQueue(float DeltaTime)
{
	if (StatesQueue.Num() == 0)
	{
		if (CurrentState->Fidget > 0)
		{
			ApplyFidgeting(DeltaTime, CurrentState);

			for (const TPair<FName, float>& pair : CurrentState->State)
			{
				ThirdPersonMesh->SetMorphTarget(FName(pair.Key), CurrentState->State[pair.Key], false);
			}
		}

		return;
	}

	if (lerpValue < 1)
		lerpValue += DeltaTime * CurrentState->TransitionSpeed;
	else
	{
		if (StatesQueue.Num() > 0)
		{
			StatesQueue[0]->HoldDuration -= DeltaTime;

			if (StatesQueue[0]->HoldDuration < 0)
			{
				CurrentState = StatesQueue[0];
				CurrentState->Fidget = StatesQueue[0]->Fidget;
				StatesQueue.RemoveAt(0);
				lerpValue = 0;
				return;
			}
		}
	}

	if (StatesQueue[0]->Fidget > 0)
	{
		ApplyFidgeting(DeltaTime, StatesQueue[0]);
	}

	lerpValue = FMath::Clamp(lerpValue, 0.f, 1.f);

	for (const TPair<FName, float>& pair : CurrentState->State)
	{
		float targetValue = FMath::Lerp(CurrentState->State[pair.Key], StatesQueue[0]->State[pair.Key], lerpValue);
		ThirdPersonMesh->SetMorphTarget(FName(pair.Key), targetValue, false);

		if (pair.Key == "Close")
		{
			FirstPersonEyeSocket->SetMorphTarget(FName("Closed"), targetValue, false);
		}
	}
}


/// <summary>
/// Blink. Doesn't overwrite current emotion.
/// </summary>
void USTK_EyeComponent::Blink(float duration, float speed)
{
	if (duration < 0 || speed < 0)
		return;

	StateData* blinkdata = new StateData();
	blinkdata->State = MakeGestureMap("Close", 1.f);
	blinkdata->HoldDuration = duration;
	blinkdata->ActiveState = FName("Close");
	blinkdata->TransitionSpeed = speed;
	blinkdata->Fidget = 0;

	lerpValue = 0;

	StateData* currentdata = new StateData();
	*currentdata = *CurrentState; // Copy all the info

	if (StatesQueue.Num() > 0)
		currentdata->Fidget = StatesQueue[0]->Fidget;
	else
		currentdata->Fidget = CurrentState->Fidget;

	currentdata->HoldDuration = CurrentState->HoldDuration;
	currentdata->TransitionSpeed = speed;
	currentdata->ActiveState = CurrentState->ActiveState;
	currentdata->State = GetCurrentState(); // Read actual mesh state

	CurrentState->State = currentdata->State;	// Update "currentstate" data to match mesh state cause it's what's actually used to lerp
	CurrentState->HoldDuration = duration;	// Set blink duration and speed
	CurrentState->TransitionSpeed = speed;
	CurrentState->Fidget = 0;

	StatesQueue.Insert(currentdata, 0); // Add em to the list. push current state to 1
	StatesQueue.Insert(blinkdata, 0); // push blink to 0
}


/// <summary>
/// Add a blank expression to the gesture queue.
/// </summary>
void USTK_EyeComponent::BlankFace(float duration, float speed)
{
	if (duration < 0 || speed < 0)
		return;

	StateData* statedata = new StateData();
	statedata->HoldDuration = duration;
	statedata->TransitionSpeed = speed;
	statedata->State = MakeGestureMap("Neutral", 1.f);

	lerpValue = 0;
	StatesQueue.Add(statedata);
}


/// <summary>
/// Fidgets the intensity of the current emotion to inject some life.
/// Fidgeting works best with simple single emotional states.
/// </summary>
void USTK_EyeComponent::ApplyFidgeting(float DeltaTime, StateData* StateToFidget)
{

	if (StateToFidget == nullptr)
		return;

	fidgetTime += DeltaTime;

	if (fidgetLerpFactor < 1)
		fidgetLerpFactor += DeltaTime * fidgetLerpSpeed;
	else
		fidgetLerpFactor = 1;

		StateToFidget->State[StateToFidget->ActiveState] = FMath::Lerp(
		StateToFidget->State[StateToFidget->ActiveState],
		fidgetRecordedIntensity,
		fidgetLerpFactor
		);

	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("Fidgeted from %f to %f"), fidgetOriginalIntensity, fidgetRecordedIntensity));

	if (fidgetTime > 1 / (5 * StateToFidget->Fidget))
	{
		fidgetTime = 0;

		if (FMath::RandRange(0, 512) < 128)
		{
			float newIntensity = fidgetOriginalIntensity;
			newIntensity += FMath::RandRange(-0.5f, 0.5f) * sin(fidgetOriginalIntensity * PI)  * (StateToFidget->Fidget);
			newIntensity = FMath::Clamp(newIntensity, 0.0f, 1.0f);

			fidgetRecordedIntensity = newIntensity;

			fidgetLerpSpeed = FMath::RandRange(2.0f, 3.5f);
			fidgetLerpFactor = 0;
		}
	}
}


void USTK_EyeComponent::SetState(std::string Name, float Intensity)
{
	ThirdPersonMesh->SetMorphTarget(FName(Name.c_str()), Intensity, false);

	if (Name == "Close")
	{
		FirstPersonEyeSocket->SetMorphTarget(FName("Closed"), Intensity, false);
	}
}

/// <summary>
/// Get how closed the eyes are at the moment. 0 is open, 1 is closed. Can be improved.
/// </summary>
float USTK_EyeComponent::GetCurrentEyeClosedLevel()
{
	// TODO, LOW PRIORITY
	// Modify this value based on what state we're in
	// E.G. Happy at intensity 1 is way more open than Angry at intensity 1
	if (StatesQueue.Num() > 0)
		return StatesQueue[0]->State[StatesQueue[0]->ActiveState];
	else
		return CurrentState->State[CurrentState->ActiveState];
}


/// <summary>
/// Get how intense a specific morph target currently is.
/// </summary>
float USTK_EyeComponent::GetValueOfSpecificMorphTarget(std::string name)
{
	if (StatesQueue.Num() > 0)
		return StatesQueue[0]->State[name.c_str()];
	else
		return CurrentState->State[name.c_str()];
}


/// <summary>
/// Overloaded. Add an emotion to the gesture queue.
/// </summary>
void USTK_EyeComponent::SetEmotion(std::string Name, float intensity, float duration, float transitionSpeed, float fidget)
{
	if (duration < 0 || transitionSpeed < 0)
		return;

	StateData* statedata = new StateData();
	statedata->State = MakeGestureMap(Name, intensity);
	statedata->HoldDuration = duration;
	statedata->TransitionSpeed = transitionSpeed;
	statedata->ActiveState = Name.c_str();
	statedata->Fidget = fidget;

	fidgetRecordedIntensity = intensity;
	fidgetOriginalIntensity = intensity;
	fidgetLerpFactor = 1.0f;

	lerpValue = 0;
	fidgetTime = 0;
	StatesQueue.Add(statedata);
}


/// <summary>
/// Overloaded. Add an emotion to the gesture queue.
/// </summary>
void USTK_EyeComponent::SetEmotion(std::string Name, float intensity, float duration, float transitionSpeed)
{
	SetEmotion(Name, intensity, duration, transitionSpeed, 0.0f); // <- default fidget
}


/// <summary>
/// Overloaded. Add an emotion to the gesture queue.
/// </summary>
void USTK_EyeComponent::SetEmotion(std::string Name, float intensity, float duration)
{
	SetEmotion(Name, intensity, duration, 2.0f); // <- default transition speed
}


/// <summary>
/// Overloaded. Add an emotion to the gesture queue.
/// </summary>
void USTK_EyeComponent::SetEmotion(std::string Name, float intensity)
{
	SetEmotion(Name, intensity, 1.f); // <- default duration
}


/// <summary>
/// Overloaded. Add an emotion to the gesture queue.
/// </summary>
void USTK_EyeComponent::SetEmotion(std::string Name)
{
	SetEmotion(Name, 1.f); // <- default intensity
}
