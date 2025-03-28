// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "DGCharacter.h"
#include "DGProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ADGCharacter

ADGCharacter::ADGCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	/*Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));*/

	// Create a gun mesh component
	/*FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);*/

	/*FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));*/
}

void ADGCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	//FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Weapons
	if (UWorld* const World = GetWorld())
	{
		for (TSubclassOf<ADGWeapon>& WeaponClass : WeaponClasses)
		{
			if (WeaponClass)
			{
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				ADGWeapon* WeaponActor = World->SpawnActor<ADGWeapon>(WeaponClass, ActorSpawnParams);
				WeaponActor->AttachToComponent(FirstPersonCameraComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));

				mSelectedWeapon = WeaponActor;
			}
		}
	}

	// Health
	mHealth = 50.0f; // #TODO: Should be mDefaultHealth
}

void ADGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateAimedActor();
}

void ADGCharacter::UpdateAimedActor()
{
	AimedActor = nullptr;

	if (mSelectedWeapon)
	{
		FVector CameraLocation = FirstPersonCameraComponent->GetComponentLocation();
		FVector CameraForwardVector = FirstPersonCameraComponent->GetForwardVector();
		FVector TraceEnd = CameraLocation + CameraForwardVector * mSelectedWeapon->GetRange();

		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);

		FHitResult HitResult;

		if (GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECollisionChannel::ECC_GameTraceChannel2, TraceParams, FCollisionResponseParams::DefaultResponseParam))
		{
			AimedActor = HitResult.GetActor(); // Not sure if it's safe to store but for my purpose I don't care
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADGCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ADGCharacter::BeginFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ADGCharacter::EndFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ADGCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADGCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ADGCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ADGCharacter::LookUpAtRate);
}

void ADGCharacter::BeginFire()
{
	if (mSelectedWeapon)
	{
		mSelectedWeapon->BeginFire();
	}
	// try and fire a projectile
	/*if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<ADGProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}*/

	// try and play the sound if specified
	/*if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}*/

	// try and play a firing animation if specified
	/*if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}*/
}

void ADGCharacter::EndFire()
{
	if (mSelectedWeapon)
	{
		mSelectedWeapon->EndFire();
	}
}

void ADGCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ADGCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ADGCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADGCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ADGCharacter::AddHealth(float Value)
{
	if (mHealth >= mDefaultHealth)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Too much of a health")));
		return false;
	}

	mHealth += Value;
	
	if (mHealth > mDefaultHealth)
	{
		mHealth = mDefaultHealth;
	}

	// TODO: clamp instead of >
	// TODO: die if 0 health (maybe in tick)

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("%f"), mHealth));

	return true;
}

float ADGCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	mHealth -= ActualDamage;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("%f"), mHealth));
	return ActualDamage;
}

FVector ADGCharacter::GetCameraAimingPoint()
{
	FVector CameraLocation = FirstPersonCameraComponent->GetComponentLocation();
	FVector CameraForwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector TraceEnd = CameraLocation + CameraForwardVector * 200.0f * 100.0f;

	FVector AimingPoint = TraceEnd;

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	FHitResult HitResult;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECollisionChannel::ECC_GameTraceChannel2, TraceParams, FCollisionResponseParams::DefaultResponseParam))
	{
		AimingPoint = HitResult.ImpactPoint;
	}

	//DrawDebugLine(GetWorld(), CameraLocation, AimingPoint, FColor::Green, false, 2.0f);

	return AimingPoint;
}
