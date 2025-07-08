// Attribution: Barbara Bugajak 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TintHandler.generated.h"

UCLASS()
class ARENA_API ATintHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATintHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTintHandler);
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Juice")
	FOnTintHandler PlayerDamage;
	
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Juice")
	FOnTintHandler PlayerLowHealthOn;

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Juice")
	FOnTintHandler PlayerLowHealthOff;
};
