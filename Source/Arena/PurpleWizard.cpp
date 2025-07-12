// Attribution: Barbara Bugajak 


#include "PurpleWizard.h"
#include "AIController.h"
#include "DetourCrowdAIController.h"
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
	bCanMeleeAttack = true;
	bIsMeleeAttacking = false;

	PushValue = 5000.0f;

}

void APurpleWizard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ADetourCrowdAIController* AIController = Cast<ADetourCrowdAIController>(GetController());
	
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (AIController)
	{
		AIController->MoveToActor(PlayerCharacter, 40, false);
		AIController->SetFocus(PlayerCharacter);
	}
		if (bCanMeleeAttack && !bIsMeleeAttacking & !bIsMeleeAttacking)
		{
			Melee(50.0f);
		}

	
}


