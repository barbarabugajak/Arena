// Attribution: Barbara Bugajak 


#include "MagicProjectile.h"

#include "DamageInterface.h"
#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMagicProjectile::AMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));
	CollisionComponent->InitSphereRadius(20.0f);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetupAttachment(CollisionComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovement->SetUpdatedComponent(CollisionComponent);
	ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = 2000.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;



	
}

// Called when the game starts or when spawned
void AMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMagicProjectile::HandleOverlap);
	
}

// Called every frame
void AMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMagicProjectile::LaunchTo(FVector WhereTo)
{
	UE_LOG(LogTemp, Display, TEXT("Launching to %s"), *WhereTo.ToString());
	DrawDebugBox(GetWorld(),WhereTo, FVector(10.0f, 10.0f, 10.0f), FColor::Red, true, 1000.0f );
	ProjectileMovement->Velocity = WhereTo * ProjectileMovement->InitialSpeed;
}

void AMagicProjectile::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                       				   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                       				   bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (!OtherActor->ActorHasTag("Player"))
		{
			UE_LOG(LogTemp, Display, TEXT("Overlapper: %s"), *OtherActor->GetClass()->GetName());
			
			if (IDamageInterface* Hittable = Cast<IDamageInterface>(OtherActor))
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit by a Magic Projectile"));	
				Hittable->ReceiveDamage(5.0f, "Magic");
			}

			Destroy();
		}
	}
}


