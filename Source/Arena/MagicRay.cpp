// Attribution: Barbara Bugajak 


#include "MagicRay.h"

#include "DamageInterface.h"
#include "InputAction.h"
#include "Components/BoxComponent.h"

// Sets default values
AMagicRay::AMagicRay()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent  = BoxComponent;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(BoxComponent);
	
	
}

// Called when the game starts or when spawned
void AMagicRay::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->SetGenerateOverlapEvents(true);
	
	OnActorBeginOverlap.AddDynamic(this, &AMagicRay::HandleOverlap);
	
}

// Called every frame
void AMagicRay::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMagicRay::HandleOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Display, TEXT("AMagicRay::HandleOverlap"));
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UDamageInterface::StaticClass()))
		{
			if (IDamageInterface* Actor = Cast<IDamageInterface>(OtherActor))
			{
				if (!OverlappedActors.Contains(OtherActor))
				{
					UE_LOG(LogTemp, Display, TEXT("AMagicRay::HandleOverlap - DamageInterface"));
					Actor->ReceiveDamage(6.0f, "Magic");
				} else
				{
					OverlappedActors.Add(OtherActor);
				}
				
			}
		}
	}
	
}


