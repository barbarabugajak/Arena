// Attribution: Barbara Bugajak 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARENA_API IDamageInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	public:
		virtual void ReceiveDamage(float DamageAmount, FString DamageType);
		virtual void CauseDamageToAnotherActor(AActor* OtherActor, float DamageAmount, FString DamageType);
};
