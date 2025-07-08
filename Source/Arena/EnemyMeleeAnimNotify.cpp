// Attribution: Barbara Bugajak 


#include "EnemyMeleeAnimNotify.h"

#include "EnemyBase.h"

void UEnemyMeleeAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp == nullptr) return;

	if (AEnemyBase* Enemy = Cast<AEnemyBase>(MeshComp->GetOwner()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner Name: %s"), *MeshComp->GetOwner()->GetName());
		Enemy->EndMeleeAttack();
		
	}
}
