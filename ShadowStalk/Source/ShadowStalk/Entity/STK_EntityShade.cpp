// Fill out your copyright notice in the Description page of Project Settings.

#include "STK_EntityShade.h"
#include "Components/SkeletalMeshComponent.h"

//Eyes
#include "Components/RectLightComponent.h"
#include "STK_EyeComponent.h"

// Sets default values
ASTK_EntityShade::ASTK_EntityShade()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetupEyes();
}

// Called when the game starts or when spawned
void ASTK_EntityShade::BeginPlay()
{
	Super::BeginPlay();

	// here's how to use the eyes. each new emotion gets added to a queue to be emoted.
	// order is: Emotion name, Emotion intensity, Duration, Transition speed, and fidgeting intensity.

	m_pEyes->SetEmotion("Close"	, 1.0f, 1, 2.0f);
	m_pEyes->SetEmotion("Angry"	, 0.6f, 5, 2.0f);
	m_pEyes->SetEmotion("Happy"	, 1.0f, 2, 2.0f);
	m_pEyes->SetEmotion("Angry"	, 1.0f, 5, 2.0f);
	m_pEyes->SetEmotion("Happy"	, 1.0f, 5, 2.0f);
	m_pEyes->SetEmotion("Sad"	, 0.6f, 5, 2.0f);
	m_pEyes->SetEmotion("Sad"	, 1.0f, 5, 2.0f);
	m_pEyes->SetEmotion("Close"	, 0.5f, 1, 2.0f, 0.5f);

}

void ASTK_EntityShade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTK_EntityShade::SetupEyes()
{
	m_pEyes = CreateDefaultSubobject<USTK_EyeComponent>(TEXT("Eyes"));
	m_pEyes->SetupMesh(m_MeshComp);

	m_pLSpotlight = CreateDefaultSubobject<URectLightComponent>(TEXT("m_pLSpotlight"));
	m_pRSpotlight = CreateDefaultSubobject<URectLightComponent>(TEXT("m_pRSpotlight"));

	m_pLSpotlight->SetRelativeLocation(FVector(-4.358307, 1.676558, -7.412248));
	m_pLSpotlight->SetRelativeRotation(FRotator(79.999390, 269.600494, 360.000641));

	m_pRSpotlight->SetRelativeLocation(FVector(4.358307, 1.676558, -7.412248));
	m_pRSpotlight->SetRelativeRotation(FRotator(79.999390, 269.600494, 360.000641));

	m_pRSpotlight->Intensity = 12000.f;
	m_pLSpotlight->Intensity = 12000.f;

	m_pRSpotlight->AttenuationRadius = 2500.f;
	m_pLSpotlight->AttenuationRadius = 2500.f;

	m_pRSpotlight->SourceWidth = 4.823f;
	m_pLSpotlight->SourceWidth = 4.823f;

	m_pRSpotlight->SourceHeight = 9.f;
	m_pLSpotlight->SourceHeight = 9.f;

	m_pRSpotlight->BarnDoorAngle = 0;
	m_pLSpotlight->BarnDoorAngle = 0;

	m_pRSpotlight->BarnDoorLength = 12.f;
	m_pLSpotlight->BarnDoorLength = 12.f;

	m_pRSpotlight->VolumetricScatteringIntensity = 0.01f;
	m_pLSpotlight->VolumetricScatteringIntensity = 0.01f;

	//FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	m_pLSpotlight->AttachToComponent(m_MeshComp, FAttachmentTransformRules::KeepRelativeTransform, TEXT("headSocket"));
	m_pRSpotlight->AttachToComponent(m_MeshComp, FAttachmentTransformRules::KeepRelativeTransform, TEXT("headSocket"));
}