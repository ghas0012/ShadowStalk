// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShadowStalk/ShadowStalk.h"
#include "GameFramework/Character.h"
#include "STK_EntityData.h"
#include "STK_EntityCharacter.generated.h"

UCLASS()
class SHADOWSTALK_API ASTK_EntityCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTK_EntityCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity|Base")
		class UCameraComponent* m_CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity|Base")
		class USkeletalMeshComponent* m_TPMeshComp;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity|Base")
		FSTK_EntityData m_MovementData;

	TSubclassOf<class UUserWidget> PauseMenuClass;
	class USTK_UWPauseMenu* UWPauseMenu;

	UPROPERTY(Replicated)
	uint8 InputLockFlags = 0;

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	UPROPERTY(Replicated, BlueprintReadOnly)
	FRotator CurrentControllerFacing;

protected:

	// position override variables
	bool bPositionOverride = false;
	float PositionOverridePercent = 0;
	FVector PositionOverrideTarget;
	FVector PositionOverrideOrigin;
	float PositionOverrideSpeed = 3.f;

	// camera override variables
	bool bCameraOverride = false;
	float CameraOverridePercent = 0;
	class USceneComponent* CameraOverrideTarget;
	FRotator CameraOverrideOrigin;
	float CameraOverrideSpeed = 4.f;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION()
		void NextItem() {};

	UFUNCTION()
		void PrevItem() {};

	UFUNCTION()
		void UseItem() {};

	class ASTK_PickupBase* CurrentItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Entity|Inventory", meta = (AllowPrivateAccess = "true"))
		class USTK_InventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Entity|Audio", meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Entity|Audio", meta = (AllowPrivateAccess = "true"))
		class USoundBase* FootstepsSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Entity|Audio", meta = (AllowPrivateAccess = "true"))
		class USoundBase* FootstepsSound2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Entity|Audio", meta = (AllowPrivateAccess = "true"))
		float FootstepFrequency;

	float FootstepTimer;

	bool PlayFootstep1 = true;
	bool PlayFootstep2 = false;

public:	

/// Input functions
#pragma region
	virtual void Forward(float value);

	virtual void Strafe(float value);

	virtual void Interact();

	virtual void Jump();

	virtual void Sprint(bool IsSprint);

	virtual void Crawl(bool IsCrawl);

	virtual void MouseLook_Vertical(float value);

	virtual void MouseLook_Horizontal(float value);

	virtual void PauseMenu();
#pragma endregion

	void LockCameraLookat(USceneComponent* SceneComp);
	void UnlockCameraLookat();

	UFUNCTION(Client, Reliable)
	void Client_LockCameraLookat(USceneComponent* SceneComp);

	void ForceMoveToPoint(FVector target);

	UFUNCTION(BlueprintCallable)
		virtual EEntityType GetEntityType() { return EEntityType::Undefined; };

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandlePositionOverride(float DeltaTime);
	void HandleCamera(float DeltaTime);

	void HandleFootstepSounds(float DeltaTime);

	void SetInputLock(EInputLockFlags flag, bool lock);
	void SetInputLock(uint8 flag, bool lock);

	UFUNCTION(Server, Reliable)
	void Server_SetInputLock(uint8 flag, bool lock);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
