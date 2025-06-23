// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	CameraComp->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	SpringArm->TargetArmLength = 300.0f;
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
		EnhancedInput->BindAction(IA_Forward, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveForward);
		EnhancedInput->BindAction(IA_Right, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveRight);
	}

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

