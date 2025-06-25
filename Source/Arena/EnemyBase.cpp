// Attribution: Barbara Bugajak 


#include "EnemyBase.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBase::ReceiveDamage(float DamageAmount, FString DamageType)
{
	if (DamageAmount > 0)
	{
		Health -= DamageAmount;
	}
	if (Health <= 0)
	{
		bool bIsDead = true;
		
	}
}

void AEnemyBase::CauseDamageToAnotherActor(AActor* OtherActor, float DamageAmount, FString DamageType)
{
	if (OtherActor != nullptr)
	{
		if (IDamageInterface* DamageTarget = Cast<IDamageInterface>(OtherActor))
		{
			DamageTarget->ReceiveDamage(DamageAmount, DamageType);
		}
	}
}


void AEnemyBase::Melee(float Distance , float DamageAmount)
{
	if (bCanMeleeAttack && !bIsMeleeAttacking)
	{
		TArray<AActor*> OverlappingActors = bIsPlayerNearby(Distance);

		if (OverlappingActors.Num() > 0)
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(OverlappingActors[0]); // Well, there's only one player
			if (Player)
			{
				// Melee Attack
				UE_LOG(LogTemp, Warning, TEXT("PurpleWizard Starting Melee Attack"));
				bIsMeleeAttacking = true;
				CauseDamageToAnotherActor(Player, DamageAmount, "Melee");
			}
		}
	}
}


void AEnemyBase::EndMeleeAttack()
{
	bIsMeleeAttacking = false;

	FTimerHandle TimerHandle;
	// Cooldown
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[this]()
		{
			bCanMeleeAttack = true;
		}, MeleeAttackDelay, false);
}


TArray<AActor*> AEnemyBase::bIsPlayerNearby(float Distance)
{
	TArray<AActor*> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	TArray < TEnumAsByte < EObjectTypeQuery > > ObjectTypes;
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(Cast<AActor>(this));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)); // Pawn collision channel
	
	bool bIsPlayerNear = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetActorLocation(),
		Distance,
		ObjectTypes,
		APlayerCharacter::StaticClass(),
		IgnoredActors,
		HitResults);
	
	return HitResults;
}

