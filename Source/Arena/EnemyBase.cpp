// Attribution: Barbara Bugajak 


#include "EnemyBase.h"

#include "MagicRay.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	if (bCanMagicRayAttack && !bIsDead)
	{
		float RandDelayFactor = FMath::RandRange(-0.2, 0.9);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			[this]()
			{
				MagicRayAttack(900.0f, 100.0f, 0.9f);
			}, RandDelayFactor, false);
	}

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
		UE_LOG(LogTemp, Warning, TEXT("Damage Amount: %f"), DamageAmount);
		UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);

		// Be pushed away
		APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Player)
		{
			UE_LOG(LogTemp, Warning, TEXT("Is pushed away"));
			GetCharacterMovement()->Launch(GetActorForwardVector()*-PushValue);
		}

		// Damage indicator
		DamageIndicatorDelegate.Broadcast(DamageAmount);

	}
	if (Health <= 0)
	{
		bIsDead = true;
		if (RayRef)
		{
			RayRef->Destroy();
		}
		FTimerHandle TimerHandle;

		SetActorEnableCollision(false);
		
		
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			[this]()
			{
				Destroy();
			}, 2.0f, false);
		
		
		
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


void AEnemyBase::Melee(float Distance)
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
				MeleeSound.Broadcast();
				bIsMeleeAttacking = true;
				bCanMeleeAttack = false;
			}
		}
	}
}


void AEnemyBase::EndMeleeAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("End of Melee Attack"));
	bIsMeleeAttacking = false;

	TArray<AActor*> OverlappingActors = bIsPlayerNearby(45.0f);
	

	if (OverlappingActors.Num() > 0)
	{
		CauseDamageToAnotherActor(OverlappingActors[0], MeleeAttackValue, "Melee");
	}
	
	
	FTimerHandle TimerHandle;
	// Cooldown
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[this]()
		{
			bCanMeleeAttack = true;
		}, MeleeAttackDelay, false);
}

void AEnemyBase::MagicRayAttack(float Range, float Disortion, float Delay)
{
	if (bCanMagicRayAttack)
	{
		UE_LOG(LogTemp, Log, TEXT("Magic Ray Attack!"))
		TArray<AActor*> Player = bIsPlayerNearby(Range);
		if (Player.Num() > 0)
		{
			if (Player[0]->IsValidLowLevel())
			{
				// Add some randomness
				FVector Location = Player[0]->GetActorLocation() + FVector(FMath::RandRange(-Disortion, Disortion), FMath::RandRange(-Disortion, Disortion), 0);;
				
				AMagicRay* Ray = GetWorld()->SpawnActor<AMagicRay>(MyBPClass, Location, FRotator::ZeroRotator);
				if (Ray)
				{
					Ray->OnSpawned.Broadcast(Delay);
					RayRef = Ray;
				}
					
				bCanMagicRayAttack = false;
				
				FTimerHandle TimerHandle;
				// Cooldown
				GetWorld()->GetTimerManager().SetTimer(
				TimerHandle,
				[this]()
				{
					bCanMagicRayAttack = true;
				}, 3.0f, false);
			}
		}
	}
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
		GetActorLocation() + GetActorForwardVector() * Distance,
		Distance,
		ObjectTypes,
		APlayerCharacter::StaticClass(),
		IgnoredActors,
		HitResults);

	// DrawDebugSphere(GetWorld(), GetActorLocation() + GetActorForwardVector() * Distance, Distance, 32, FColor::Red, false, 1.0f);
	
	return HitResults;
}

