// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	CameraComp->bUsePawnControlRotation = false;

	SpringArm->SetupAttachment(GetCapsuleComponent());
	CameraComp->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 300.0f;
	
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	UE_LOG(LogTemp, Warning, TEXT("MovementMode: %d"), GetCharacterMovement()->MovementMode);


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
		UE_LOG(LogTemp, Warning, TEXT("Hit something"));
	}
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
