// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Jeffrey Armstrong

// Changelog:
// - Class init.
// = Renamed class to add STK_ prefix

#include "STK_InventoryComponent.h"
#include "../Pickups/STK_PickupBase.h"
#include "../Entity/STK_Entity.h"

// Sets default values for this component's properties
USTK_InventoryComponent::USTK_InventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void USTK_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}


/// <summary>
/// Adds a certain pickup to the inventory and disables the picked up item.
/// </summary>
void USTK_InventoryComponent::AddToInventory(ASTK_PickupBase* pickup)
{
	Inventory.AddUnique(pickup);

	pickup->Disable();
}

int USTK_InventoryComponent::GetInventoryCount()
{
	return Inventory.Num();
}


/// <summary>
/// Cycle to the next item in the inventory.
/// </summary>
void USTK_InventoryComponent::NextInventoryItem()
{
	SelectInventory(E_InventoryDirection::NEXT);
}


/// <summary>
/// Cycle to the previous item in the inventory.
/// </summary>
void USTK_InventoryComponent::PreviousInventoryItem()
{
	SelectInventory(E_InventoryDirection::PREV);
}


ASTK_PickupBase* USTK_InventoryComponent::GetCurrentInventory() const
{
	return CurrentInventoryItem;
}


void USTK_InventoryComponent::ResetCurrentInventory()
{
	CurrentInventoryItem = nullptr;
}


/// <summary>
/// Equips a specific item.
/// </summary>
void USTK_InventoryComponent::EquipNewInventoryItem(ASTK_PickupBase* NewItem)
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


/// <summary>
/// Helper function to move through the inventory in a specified direction.
/// </summary>
void USTK_InventoryComponent::SelectInventory(E_InventoryDirection direction)
{
	if (Inventory.Num() == 0)
	{
		return;
	}

	ASTK_PickupBase* InventoryItemToEquip = nullptr;

	int32 Index = (direction == E_InventoryDirection::NEXT ? 0 : Inventory.Num() - 1);

	if (CurrentInventoryItem)
	{
		Inventory.Find(CurrentInventoryItem, Index);

		Index += (direction == E_InventoryDirection::NEXT ? 1 : -1);
	}

	if (Index < Inventory.Num() && Index > -1)
	{
		InventoryItemToEquip = Inventory[Index];

		EquipNewInventoryItem(InventoryItemToEquip);
	}
}


// Called every frame
void USTK_InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

