// Attribution: Barbara Bugajak 

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "MagicRay.generated.h"

UCLASS()
class ARENA_API AMagicRay : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagicRay();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Juice")
	float VisualDelay = 0.2f;
	
	UFUNCTION()
	void HandleOverlap(AActor* MyOverlappedActor, AActor* OtherActor);

	TArray<AActor*> OverlappedActors; // Hit each actor only once, for gameplay flow reason;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpawned, float, Delay);
	UPROPERTY(EditAnywhere, BlueprintAssignable, BlueprintReadWrite, Category = "Init")
	FOnSpawned OnSpawned;
};
