// Attribution: Barbara Bugajak 


#include "TintHandler.h"

// Sets default values
ATintHandler::ATintHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATintHandler::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATintHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

