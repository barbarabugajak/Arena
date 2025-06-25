// Attribution: Barbara Bugajak 

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EnemyMeleeAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class ARENA_API UEnemyMeleeAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify ( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
	
};
