// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageInterface.h"
#include "EnemyBase.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
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
	
	UFUNCTION()
	void MoveForward(const FInputActionValue& Value);

	UFUNCTION()
	void MoveRight(const FInputActionValue& Value);

	// Attack
	UFUNCTION()
	TArray<AActor*> EnemiesNearby(float Distance);
	
	UFUNCTION()
	void MeleeAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UInputAction* IA_Attack;

	// Block
	UFUNCTION()
	void StartBlocking(const FInputActionValue& Value);

	UFUNCTION()
	void StopBlocking(const FInputActionValue& Value);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBlockingStateChanged);
	UPROPERTY(BlueprintAssignable, Category = "Blocking")
	FOnBlockingStateChanged BlockChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocking")
	float MaxBlock = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocking")
	float BlockMeter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocking")
	bool bIsBlocking = false;
	
	// Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth;

	// Wizard Comps
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wizard Components")
	UStaticMeshComponent* ShieldComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wizard Components")
	UInputAction* IA_Shield;
	
	// Damage
	virtual void ReceiveDamage(float DamageAmount, FString DamageType) override;
	virtual void CauseDamageToAnotherActor(AActor* OtherActor, float DamageAmount, FString DamageType) override;
};
