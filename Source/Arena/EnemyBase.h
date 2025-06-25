// Attribution: Barbara Bugajak 

#pragma once

#include "CoreMinimal.h"
#include "DamageInterface.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UCLASS()
class ARENA_API AEnemyBase : public ACharacter
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

	UFUNCTION()
	virtual void TakeSomeDamage(float DamageAmount, bool bIsMagicalDamage);

	UFUNCTION()
	TArray<AActor*> bIsPlayerNearby(float Distance);

	UFUNCTION()
	virtual void EndMeleeAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bIsMeleeAttacking;
};
