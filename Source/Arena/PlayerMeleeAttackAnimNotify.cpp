// Attribution: Barbara Bugajak 


#include "PlayerMeleeAttackAnimNotify.h"

#include "PlayerCharacter.h"

void UPlayerMeleeAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp == nullptr) return;

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner()))
	{
		Player->EndMeleeAttack();
	}
}
