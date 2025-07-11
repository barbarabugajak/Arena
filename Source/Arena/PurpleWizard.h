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
class ARENA_API APurpleWizard : public AEnemyBase
{
	GENERATED_BODY()

	public:
		APurpleWizard();

		void BeginPlay() override;
	
		// Called every frame
		virtual void Tick(float DeltaTime) override;
		

};
