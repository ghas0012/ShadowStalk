// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Jeffrey Armstrong

// Changelog:
// - Class init.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STK_InventoryComponent.generated.h"

UENUM(BlueprintType)
enum class E_InventoryDirection : uint8
{
	NEXT = 0 UMETA(DisplayName = "NEXT"),
	PREV = 1 UMETA(DisplayName = "PREV")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHADOWSTALK_API USTK_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTK_InventoryComponent();

	void AddToInventory(class ASTK_PickupBase* pickup);

	int GetInventoryCount();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		virtual void NextInventoryItem();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		virtual void PreviousInventoryItem();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		class ASTK_PickupBase* GetCurrentInventory() const;

	void ResetCurrentInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	class ASTK_EntityShade* CharacterOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		class ASTK_PickupBase* CurrentInventoryItem;

	void EquipNewInventoryItem(ASTK_PickupBase* NewItem);

	void SelectInventory(E_InventoryDirection direction);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray <class ASTK_PickupBase*>	Inventory;
};
