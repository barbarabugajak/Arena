// Attribution: Barbara Bugajak 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealingDevice.generated.h"

UCLASS()
class ARENA_API AHealingDevice : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealingDevice();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
