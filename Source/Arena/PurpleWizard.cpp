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

void APurpleWizard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AAIController* AIController = Cast<AAIController>(GetController());
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (AIController->IsFollowingAPath() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Followin a path"));
		FVector SomeRandomnessToMovement = FVector(FMath::RandRange(-10, 10), FMath::RandRange(-10, 10), 0);
		AIController->MoveToLocation(PlayerCharacter->GetActorLocation()+SomeRandomnessToMovement, 40);
		AIController->SetFocus(PlayerCharacter);
	}
	
		if (bCanMeleeAttack && !bIsAttacking)
		{
			TArray<AActor*> OverlappingActors = bIsPlayerNearby(100.0f);

			if (OverlappingActors.Num() > 0)
			{
				APlayerCharacter* Player = Cast<APlayerCharacter>(OverlappingActors[0]); // Well, there's only one player
				if (Player)
				{
					// Melee Attack
					UE_LOG(LogTemp, Warning, TEXT("PurpleWizard Starting Melee Attack"));
					Melee();
				}
			}
		} 
}

void APurpleWizard::Melee()
{
	bIsAttacking = true;
	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(100.0f, 100.0f, 100.0f), FColor::Cyan);
	APlayerCharacter* Player =  Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	UE_LOG(LogTemp, Log, TEXT("Melee"))
	
	CauseDamageToAnotherActor(Player, 2, "Melee");

	FTimerHandle TimerHandle;
	// Cooldown
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[this]()
		{
			bCanMeleeAttack = true;
		}, 2.0f, false);
}


// Generic
void APurpleWizard::CauseDamageToAnotherActor(AActor* OtherActor, float DamageAmount, FString DamageType)
{
	if (OtherActor != nullptr)
	{
		if (IDamageInterface* DamageTarget = Cast<IDamageInterface>(OtherActor))
		{
			DamageTarget->ReceiveDamage(DamageAmount, DamageType);
		}
	}
}

void APurpleWizard::ReceiveDamage(float DamageAmount, FString DamageType)
{
	if (DamageAmount > 0)
	{
		Health -= DamageAmount;
	}
	if (Health <= 0)
	{
		// For now
		Destroy();
	}
}