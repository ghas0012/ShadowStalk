// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_PickupSpawn.h"
#include "ShadowStalk/GameModes/STK_MatchGameMode.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

// Sets default values
ASTK_PickupSpawn::ASTK_PickupSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComp);

	EditorSpriteComponent = CreateDefaultSubobject<UBillboardComponent>("EditorSpriteComp");
	EditorSpriteComponent->bHiddenInGame = true;
	EditorSpriteComponent->bIsEditorOnly = true;
	EditorSpriteComponent->SetRelativeLocation(FVector(0, 0, 100));
	EditorSpriteComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UTexture2D> EditorSpriteGrabber_Key(TEXT("/Game/Custom_Icons/Pickup_Spawn_Icons/SpawnIcon_Key"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> EditorSpriteGrabber_Item(TEXT("/Game/Custom_Icons/Pickup_Spawn_Icons/SpawnIcon_Item"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> EditorSpriteGrabber_Undefined(TEXT("/Game/Custom_Icons/Pickup_Spawn_Icons/SpawnIcon_Undefined"));

	if (EditorSpriteGrabber_Key.Succeeded())
		Sprite_Key = EditorSpriteGrabber_Key.Object;

	if (EditorSpriteGrabber_Item.Succeeded())
		Sprite_Item = EditorSpriteGrabber_Item.Object;

	if (EditorSpriteGrabber_Undefined.Succeeded())
		Sprite_Undefined = EditorSpriteGrabber_Undefined.Object;

	switch (SpawnType)
	{
		case EPickupType::Key:
			EditorSpriteComponent->SetSprite(Sprite_Key);
			break;
		case EPickupType::Item:
			EditorSpriteComponent->SetSprite(Sprite_Item);
			break;
		default:
			EditorSpriteComponent->SetSprite(Sprite_Undefined);
			break;
	}
}

// Called when the game starts or when spawned
void ASTK_PickupSpawn::BeginPlay()
{
	
	ASTK_MatchGameMode* pGameMode = nullptr;

	if (GetWorld())
	{
		pGameMode = Cast<ASTK_MatchGameMode>(GetWorld()->GetAuthGameMode());
	}

	if (pGameMode)
	{
		pGameMode->RegisterPickupSpawnPoint(this);
	}

	Super::BeginPlay();

}
 

void ASTK_PickupSpawn::PostLoad()
{
	Super::PostLoad();

	switch (SpawnType)
	{
	case EPickupType::Key:
		EditorSpriteComponent->SetSprite(Sprite_Key);
		break;
	case EPickupType::Item:
		EditorSpriteComponent->SetSprite(Sprite_Item);
		break;
	default:
		EditorSpriteComponent->SetSprite(Sprite_Undefined);
		break;
	}
}


void ASTK_PickupSpawn::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	switch (SpawnType)
	{
	case EPickupType::Key:
		EditorSpriteComponent->SetSprite(Sprite_Key);
		break;
	case EPickupType::Item:
		EditorSpriteComponent->SetSprite(Sprite_Item);
		break;
	default:
		EditorSpriteComponent->SetSprite(Sprite_Undefined);
		break;
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

EPickupType ASTK_PickupSpawn::GetPickupType()
{
	return SpawnType;
};