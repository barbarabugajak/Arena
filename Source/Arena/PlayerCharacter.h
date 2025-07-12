// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageInterface.h"
#include "EnemyBase.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "MagicProjectile.h"
#include "MagicRay.h"
#include "TintHandler.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ARENA_API APlayerCharacter : public ACharacter, public IDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm; // I'm a fan now

	UFUNCTION()
	void CameraRotation(const FInputActionValue& Value);
	
	// Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	UInputMappingContext* IMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	UInputAction* IA_Forward;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	UInputAction* IA_Right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	UInputAction* IA_Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	UInputAction* IA_Esc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	UInputAction* IA_Potion;
	
	UFUNCTION()
	void MoveForward(const FInputActionValue& Value);

	UFUNCTION()
	void MoveRight(const FInputActionValue& Value);

	// Attack
	UFUNCTION()
	TArray<AActor*> EnemiesNearby(float Distance);
	
	UFUNCTION()
	void MeleeAttack();

	UFUNCTION()
	void EndMeleeAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bIsMeleeAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bCanMeleeAttack = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UInputAction* IA_Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UInputAction* IA_MagicRay;

	UFUNCTION()
	void MagicRayAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<AActor> MyBPClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bCanMagicRayAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UInputAction* IA_MagicProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<AMagicProjectile> MagicProjectile_BPClass;

	UFUNCTION()
	void LaunchMagicProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bCanProjectileAttack = true;
	
	// Block
	UFUNCTION()
	void StartBlocking(const FInputActionValue& Value);

	UFUNCTION()
	void StopBlocking(const FInputActionValue& Value);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBlockingStateChanged);
	UPROPERTY(BlueprintAssignable, Category = "Blocking")
	FOnBlockingStateChanged BlockChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocking")
	float MaxBlock = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocking")
	float BlockMeter = MaxBlock;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocking")
	bool bIsBlocking = false;
	
	// Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth = 100.0f;

	// Wizard Comps
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wizard Components")
	UStaticMeshComponent* ShieldComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wizard Components")
	UInputAction* IA_Shield;
	
	// Damage
	virtual void ReceiveDamage(float DamageAmount, FString DamageType) override;
	virtual void CauseDamageToAnotherActor(AActor* OtherActor, float DamageAmount, FString DamageType) override;

	// UI Handlers
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUIChanges);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, Category = "UI")
	FOnUIChanges ShowLoseScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, Category = "UI")
	FOnUIChanges ChangeAmountOfPotions;

	UFUNCTION()
	void QuitGame(const FInputActionValue& Value);

	UFUNCTION()
	void Heal(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	bool bIsHealing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	int HealAmount = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Juice")
	TSoftObjectPtr<ATintHandler> TintHandler;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSoundEffect, FString, SoundType);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, Category = "Juice")
	FSoundEffect SoundEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bIsAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Juice")
	AMagicRay* RayRef;
};
