// Attribution: Barbara Bugajak 


#include "HealingDevice.h"

// Sets default values
AHealingDevice::AHealingDevice()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHealingDevice::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealingDevice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

