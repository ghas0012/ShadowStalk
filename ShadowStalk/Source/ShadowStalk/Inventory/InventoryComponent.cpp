// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "../Pickups/STK_PickupBase.h"
#include "../Entity/STK_Entity.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UInventoryComponent::AddToInventory(ASTK_PickupBase* pickup)
{
	Inventory.AddUnique(pickup);

	pickup->Disable();
}

int UInventoryComponent::GetInventoryCount()
{
	return Inventory.Num();
}

void UInventoryComponent::NextInventoryItem()
{
	SelectInventory(EInventoryDirection::NEXT);
}

void UInventoryComponent::PreviousInventoryItem()
{
	SelectInventory(EInventoryDirection::PREV);
}

ASTK_PickupBase* UInventoryComponent::GetCurrentInventory() const
{
	return CurrentInventoryItem;
}

void UInventoryComponent::ResetCurrentInventory()
{
	CurrentInventoryItem = nullptr;
}

void UInventoryComponent::EquipNewInventoryItem(ASTK_PickupBase* NewItem)
{
	if (CurrentInventoryItem)
	{
		CurrentInventoryItem->Disable();

		CurrentInventoryItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	if (NewItem)
	{
		NewItem->Enable();

		ASTK_Entity* OwningActor = Cast<ASTK_Entity>(GetOwner());
	}

	CurrentInventoryItem = NewItem;
}

void UInventoryComponent::SelectInventory(EInventoryDirection direction)
{
	if (Inventory.Num() == 0)
	{
		return;
	}

	ASTK_PickupBase* InventoryItemToEquip = nullptr;

	int32 Index = (direction == EInventoryDirection::NEXT ? 0 : Inventory.Num() - 1);

	if (CurrentInventoryItem)
	{
		Inventory.Find(CurrentInventoryItem, Index);

		Index += (direction == EInventoryDirection::NEXT ? 1 : -1);
	}

	if (Index < Inventory.Num() && Index > -1)
	{
		InventoryItemToEquip = Inventory[Index];

		EquipNewInventoryItem(InventoryItemToEquip);
	}
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

