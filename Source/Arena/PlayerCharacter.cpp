// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
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
	SpringArm->TargetArmLength = 300.0f;
	
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
	}

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
	UE_LOG(LogTemp, Warning, TEXT("Attacking!"))
	TArray<AActor*> Enemies = EnemiesNearby(100.0f); // Seems about right :)

	if (Enemies.Num() > 0)
	{
		for (AActor* EnemyHit: Enemies)
		{
			if (IDamageInterface* HitActor = Cast<IDamageInterface>(EnemyHit))
			{
				HitActor->ReceiveDamage(10.0f, "Melee");
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
		Health -= DamageAmount;
	}
	if (Health <= 0)
	{
		// For now
		UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
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
	
	bool bIsPlayerNear = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetActorLocation(),
		Distance,
		ObjectTypes,
		AEnemyBase::StaticClass(),
		IgnoredActors,
		HitResults);

	DrawDebugSphere(GetWorld(), GetActorLocation(), Distance, 20, FColor::Yellow, true);
	return HitResults;
}
