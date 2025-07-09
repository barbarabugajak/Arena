// Attribution: Barbara Bugajak 

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "MagicRay.h"
#include "DarkWizard.generated.h"

/**
 * 
 */
UCLASS()
class ARENA_API ADarkWizard : public AEnemyBase
{
	GENERATED_BODY()

public:
	ADarkWizard();

	void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMagicRay> MagicRayClass;

	bool bShouldGetNewPath;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
