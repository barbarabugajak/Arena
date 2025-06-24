// Attribution: Barbara Bugajak 

#pragma once

#include "CoreMinimal.h"
#include "DamageInterface.h"
#include "EnemyBase.h"
#include "PurpleWizard.generated.h"

/**
 * 
 */
UCLASS()
class ARENA_API APurpleWizard : public AEnemyBase, public IDamageInterface
{
	GENERATED_BODY()

	public:
		APurpleWizard();
	
		// Called every frame
		virtual void Tick(float DeltaTime) override;
	
		// Damage
		void Melee();
		bool bIsAttacking = false;
		bool bCanMeleeAttack = true;
		virtual void ReceiveDamage(float DamageAmount, FString DamageType) override;
		virtual void CauseDamageToAnotherActor(AActor* OtherActor, float DamageAmount, FString DamageType) override;
	
		float Health = 10.0f;
};
