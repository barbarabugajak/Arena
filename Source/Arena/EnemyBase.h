// Attribution: Barbara Bugajak 

#pragma once

#include "CoreMinimal.h"
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

	// Visuals
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;
	
	// Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth;

	UFUNCTION()
	virtual void CauseDamage(AActor* OtherActor);

	UFUNCTION()
	virtual void TakeSomeDamage(float DamageAmount, bool bIsMagicalDamage);

	UFUNCTION()
	bool bIsPlayerNearby(float Distance);
};
