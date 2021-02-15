// Fill out your copyright notice in the Description page of Project Settings.


#include "STK_EntityShade.h"
#include "Components/SkeletalMeshComponent.h"
#include "STK_EyeComponent.h"

// Sets default values
ASTK_EntityShade::ASTK_EntityShade()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pEyes = CreateDefaultSubobject<USTK_EyeComponent>(TEXT("Eyes"));
	m_pEyes->SetupMesh(m_MeshComp);
}

// Called when the game starts or when spawned
void ASTK_EntityShade::BeginPlay()
{
	Super::BeginPlay();

	// here's how to use the eyes. each new emotion gets added to a queue to be emoted.
	// order is: Emotion name, Emotion intensity, Duration, Transition speed, and fidgeting intensity.

	m_pEyes->SetEmotion("Angry"	, 0.6f, 5, 2.0f, 0.2f);
	m_pEyes->SetEmotion("Happy"	, 1.0f, 2, 2.0f, 0.1f);
	m_pEyes->SetEmotion("Angry"	, 1.0f, 5, 2.0f, 0.1f);
	m_pEyes->SetEmotion("Happy"	, 1.0f, 5, 2.0f, 0.1f);
	m_pEyes->SetEmotion("Sad"	, 0.6f, 5, 2.0f, 0.2f);
	m_pEyes->SetEmotion("Sad"	, 1.0f, 5, 2.0f		 );
	m_pEyes->SetEmotion("Close"	, 0.5f, 1, 2.0f, 0.5f);

}

void ASTK_EntityShade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}