// Attribution: Barbara Bugajak 


#include "DarkWizard.h"

#include "AIController.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

ADarkWizard::ADarkWizard()
{
	PrimaryActorTick.bCanEverTick = true;

	// AI Possess
	AIControllerClass = AAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ADarkWizard::BeginPlay()
{
	Super::BeginPlay();
	Health = 15.0f;
	MaxHealth = 15.0f;
	MeleeAttackDelay = 2.0f;
	bCanMeleeAttack = false; // This one does not melee attack
	bIsMeleeAttacking = false;
	bCanMagicRayAttack = false;
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		bCanMagicRayAttack = true; // Some time for the player
	}, 1.0f, false);
	
	MyBPClass = MagicRayClass->GetDefaultObject()->GetClass();

	
}

void ADarkWizard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AAIController* AIController = Cast<AAIController>(GetController());
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	AIController->SetFocus(PlayerCharacter);
	
	if (bCanMagicRayAttack)
	{
		MagicRayAttack(100.0f, 2.0f);
	}


}