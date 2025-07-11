// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "MagicRay.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Camera
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	CameraComp->bUsePawnControlRotation = false;

	SpringArm->SetupAttachment(GetCapsuleComponent());
	CameraComp->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 400.0f;
	
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Components
	ShieldComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	ShieldComponent->SetupAttachment(GetCapsuleComponent());
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	if (ShieldComponent != nullptr)
	{
		ShieldComponent->SetVisibility(false, true);	
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Shield not accessible"));
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BlockMeter < MaxBlock && bIsBlocking == false && bIsMeleeAttacking == false)
	{
		BlockMeter += 5 * DeltaTime;
	}


	if (bIsHealing == true && bIsMeleeAttacking == false)
	{
		if (HealAmount + Health > MaxHealth)
		{
			Health = MaxHealth;
		} else
		{
			Health += HealAmount;
			if (Health > 10)
			{
				if (TintHandler)
				{
					TintHandler->PlayerLowHealthOff.Broadcast();
				}
			}
		}
		bIsHealing = false;
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* SubSys = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				UE_LOG(LogTemp, Display, TEXT("Enhanced Input Setup"));
				SubSys->AddMappingContext(IMC, 0);
			}
		}
	}
	
	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(IA_Camera, ETriggerEvent::Triggered, this, &APlayerCharacter::CameraRotation);
		EnhancedInput->BindAction(IA_Forward, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveForward);
		EnhancedInput->BindAction(IA_Right, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveRight);
		EnhancedInput->BindAction(IA_Attack, ETriggerEvent::Triggered, this, &APlayerCharacter::MeleeAttack);
		EnhancedInput->BindAction(IA_Shield, ETriggerEvent::Started, this, &APlayerCharacter::StartBlocking);
		EnhancedInput->BindAction(IA_Shield, ETriggerEvent::Completed, this, &APlayerCharacter::StopBlocking);
		EnhancedInput->BindAction(IA_MagicRay, ETriggerEvent::Triggered, this, &APlayerCharacter::MagicRayAttack);
		EnhancedInput->BindAction(IA_MagicProjectile, ETriggerEvent::Triggered, this, &APlayerCharacter::LaunchMagicProjectile);
		EnhancedInput->BindAction(IA_Esc, ETriggerEvent::Triggered, this, &APlayerCharacter::QuitGame);
		EnhancedInput->BindAction(IA_Potion, ETriggerEvent::Started, this, &APlayerCharacter::Heal);
	}

}

void APlayerCharacter::Heal(const FInputActionValue& Value)
{
	ChangeAmountOfPotions.Broadcast();
	UE_LOG(LogTemp, Log, TEXT("Healing"));
}


void APlayerCharacter::QuitGame(const FInputActionValue& Value)
{
	APlayerController* PC = Cast<APlayerController>(Controller);
	UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, false);
}

void APlayerCharacter::CameraRotation(const FInputActionValue& Value)
{
	FVector Val = Value.Get<FVector>();
	AddControllerYawInput(Val.X);
}


void APlayerCharacter::MoveForward(const FInputActionValue& Value)
{
	float FwdValue = Value.Get<float>();
	if (FwdValue != 0)
	{
		const FRotator YawRotation(0, GetControlRotation().Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, FwdValue);
	}
}

void APlayerCharacter::MoveRight(const FInputActionValue& Value)
{
	float RightValue = Value.Get<float>();
	if (RightValue != 0)
	{
		const FRotator Rot(0, GetControlRotation().Yaw, 0);
		const FVector Direction = FRotationMatrix(Rot).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, RightValue);
	}
}

void APlayerCharacter::MeleeAttack()
{
	if (bCanMeleeAttack)
	{
		
		bIsMeleeAttacking = true;
		bCanMeleeAttack = false;
		UE_LOG(LogTemp, Warning, TEXT("Attacking!"))
		TArray<AActor*> Enemies = EnemiesNearby(100.0f); // Seems about right :)
		
		if (Enemies.Num() > 0)
		{
			for (AActor* EnemyHit: Enemies)
			{
				if (IDamageInterface* HitActor = Cast<IDamageInterface>(EnemyHit))
				{
					HitActor->ReceiveDamage(5.0f, "Melee");
				}
			}
		}

		SoundEffect.Broadcast("Melee");
	}
}

void APlayerCharacter::EndMeleeAttack()
{
	bIsMeleeAttacking = false;
	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[this]()
		{
			bCanMeleeAttack = true;
		}, 0.2f, false); // Needs to be a short while, due to anims
}


void APlayerCharacter::MagicRayAttack()
{
	if (bCanMagicRayAttack)
	{
		UE_LOG(LogTemp, Log, TEXT("Magic Ray Attack!"))
		TArray<AActor*> Enemies = EnemiesNearby(600.0f);
		if (Enemies.Num() > 0)
		{
			if (Enemies[0]->IsValidLowLevel())
			{
				// Add some randomness
				FVector Location = Enemies[0]->GetActorLocation() + FVector(FMath::RandRange(-80, 80), FMath::RandRange(-80, 80), 0);;


				AMagicRay* Ray = GetWorld()->SpawnActor<AMagicRay>(MyBPClass, Location, FRotator::ZeroRotator);
				Ray->OnSpawned.Broadcast(0.3f);
				bCanMagicRayAttack = false;
				
				FTimerHandle TimerHandle;
				// Cooldown
				GetWorld()->GetTimerManager().SetTimer(
				TimerHandle,
				[this]()
				{
					bCanMagicRayAttack = true;
				}, 3.0f, false);
			}
		}
	}
}

void APlayerCharacter::LaunchMagicProjectile()
{
	
	if (bCanProjectileAttack && MagicProjectile_BPClass->IsValidLowLevel()) 
	{
		TArray<AActor*> Enemies = EnemiesNearby(800.0f);
		if (Enemies.Num() > 0)
		{
			if (Enemies[0]->IsValidLowLevel())
			{
				FVector ProjectileAim = Enemies[0]->GetActorLocation();

				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;

				FVector Location = GetMesh()->GetSocketLocation("Weapon");
				
				// DrawDebugBox(GetWorld(),Location, FVector(10.0f, 10.0f, 10.0f), FColor::Red, true, 1000.0f );
				GetWorld()->SpawnActor<AMagicProjectile>(MagicProjectile_BPClass, Location, GetActorRotation(), SpawnParams);

				bCanProjectileAttack = false;

				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
				{
					bCanProjectileAttack = true;
				}, 2.0f, false);
				
			}
		}
		
	}
}


void APlayerCharacter::StartBlocking(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Starting Blocking!"));
	bIsBlocking = true;
	// Hide the component
	ShieldComponent->SetVisibility(true, true);

	BlockChanged.Broadcast();
}

void APlayerCharacter::StopBlocking(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Ending Blocking!"));
	ShieldComponent->SetVisibility(false, true);
	bIsBlocking = false;
	BlockChanged.Broadcast();

}


void APlayerCharacter::ReceiveDamage(float DamageAmount, FString DamageType)
{
		if (DamageAmount > 0)
		{
			
			
			if (bIsBlocking)
			{
				if (DamageAmount <= BlockMeter && bIsBlocking)
				{
					BlockMeter -= DamageAmount;
				} else
				{
					bIsBlocking = false;
					BlockMeter = 0;
					StopBlocking(1);
					Health -= DamageAmount;

					if (TintHandler)
					{
						TintHandler->PlayerDamage.Broadcast();
					}

					
				}
			} else
			{
				Health -= DamageAmount;
				UE_LOG(LogTemp, Warning, TEXT("Damage Amount: %f"), DamageAmount);
				UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);

				if (TintHandler)
				{
					TintHandler->PlayerDamage.Broadcast();
				}
			}
			
		}

		if (Health <= 10)
		{
			if (TintHandler)
			{
				TintHandler->PlayerLowHealthOn.Broadcast();
			}
		}
		
		if (Health <= 0)
		{
			// UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
			bIsAlive = false;
			ShowLoseScreen.Broadcast();
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
	
}
void APlayerCharacter::CauseDamageToAnotherActor(AActor* OtherActor, float DamageAmount, FString DamageType)
{
	UE_LOG(LogTemp, Warning, TEXT("Damage: %f"), DamageAmount);
	return ;
}


TArray<AActor*> APlayerCharacter::EnemiesNearby(float Distance)
{
	TArray<AActor*> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	TArray < TEnumAsByte < EObjectTypeQuery > > ObjectTypes;
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(Cast<AActor>(this));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)); // Pawn collision channel

	FVector Location = GetActorLocation() + (GetActorForwardVector()*Distance);
	
	bool bIsPlayerNear = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetActorLocation(),
		Distance,
		ObjectTypes,
		AEnemyBase::StaticClass(),
		IgnoredActors,
		HitResults);

	// DrawDebugSphere(GetWorld(), Location, Distance, 20, FColor::Yellow, true);
	return HitResults;
}
