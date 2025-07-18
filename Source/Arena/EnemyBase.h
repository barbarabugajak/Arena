// Attribution: Barbara Bugajak 

#pragma once

#include "CoreMinimal.h"
#include "DamageInterface.h"
#include "MagicRay.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UCLASS()
class ARENA_API AEnemyBase : public ACharacter, public IDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	// Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bIsDead = false;

	virtual void ReceiveDamage(float DamageAmount, FString DamageType) override;
	
	virtual void CauseDamageToAnotherActor(AActor* OtherActor, float DamageAmount, FString DamageType) override;
	
	
	
	UFUNCTION()
	TArray<AActor*> bIsPlayerNearby(float Distance);

	UFUNCTION()
	void Melee(float Distance);
	
	UFUNCTION()
	void EndMeleeAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MeleeAttackDelay = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MeleeAttackValue = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bIsMeleeAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bCanMeleeAttack;

	UFUNCTION()
	void MagicRayAttack(float Range, float Disortion, float Delay);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<AActor> MyBPClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bCanMagicRayAttack = true;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamage, float, DamageAmount);
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "DamageIndicator")
	FOnDamage DamageIndicatorDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Juice")
	float PushValue = 1000.0f;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSoundEffect);
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Juice")
	FSoundEffect MeleeSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Juice")
	AMagicRay* RayRef;

};
