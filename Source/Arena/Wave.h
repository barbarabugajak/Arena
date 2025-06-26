// Attribution: Barbara Bugajak 

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "UObject/NoExportTypes.h"
#include "Wave.generated.h"

/**
 * 
 */


USTRUCT(BlueprintType)
struct FEnemyTypeData
{
	GENERATED_BODY()
	
	// Number of enemies to spawn in this wave
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 EnemyCount = 0;

	// The enemy class to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	TSubclassOf<AEnemyBase> EnemyClass;
	
	FEnemyTypeData() {}
};

USTRUCT(BlueprintType)
struct FWaveOfEnemies
{
	GENERATED_BODY()
	
	// Number of enemies to spawn in this wave
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	TArray<FEnemyTypeData> WhatToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int WaveID = 0;
	
	FWaveOfEnemies() {}
};


UCLASS()
class ARENA_API UWave : public UObject
{
	GENERATED_BODY()

	public:
		void Initialize();
	
		UFUNCTION(BlueprintCallable, Category = "Wave")
		void NewWave();

		// UFUNCTION()
		// void WaveEnds();

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
		int EnemiesLeft;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
		int CurrentWave;
	
};
