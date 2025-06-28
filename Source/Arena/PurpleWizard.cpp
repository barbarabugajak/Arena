// Attribution: Barbara Bugajak 


#include "PurpleWizard.h"
#include "AIController.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

APurpleWizard::APurpleWizard()
{
	PrimaryActorTick.bCanEverTick = true;

	// AI Possess
	AIControllerClass = AAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


}

void APurpleWizard::BeginPlay()
{
	Super::BeginPlay();
	Health = 25.0f;
	MaxHealth = 25.0f; 
	MeleeAttackDelay = 1.2f;
	bCanMeleeAttack = true;
	bIsMeleeAttacking = false;
}

void APurpleWizard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AAIController* AIController = Cast<AAIController>(GetController());
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (AIController)
	{
		FVector SomeRandomnessToMovement = FVector(FMath::RandRange(-10, 10), FMath::RandRange(-10, 10), 0);
		AIController->MoveToLocation(PlayerCharacter->GetActorLocation()+SomeRandomnessToMovement, 40);
		AIController->SetFocus(PlayerCharacter);
	}
		if (bCanMeleeAttack && !bIsMeleeAttacking)
		{
			Melee(100.0f, 2.0f);
		}
}


