// Attribution: Barbara Bugajak 


#include "EnemyBase.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMesh->SetupAttachment(GetRootComponent());
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

void AEnemyBase::CauseDamage(AActor* OtherActor)
{
	
}

void AEnemyBase::TakeSomeDamage(float DamageAmount, bool bIsMagicalDamage)
{
	
}

bool AEnemyBase::bIsPlayerNearby(float Distance)
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
	
	return bIsPlayerNear;
}

