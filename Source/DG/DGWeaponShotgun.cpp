
#include "DGWeaponShotgun.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"
#include "DGGameMode.h"
#include "DGCharacter.h"
#include "DGDamageTypes.h"

ADGWeaponShotgun::ADGWeaponShotgun()
{
	//
}

void ADGWeaponShotgun::BeginFire()
{
	Super::BeginFire();

	FName MuzzleSocketName = ActiveBarrel ? "MuzzleLeftSocket" : "MuzzleRightSocket";
	ActiveBarrel = !ActiveBarrel;

	if (UWorld* World = GetWorld())
	{
		FVector MuzzleLocation = mWeaponMesh->GetSocketLocation(MuzzleSocketName);

		if (FireSound != NULL)
		{
			UGameplayStatics::PlaySound2D(this, FireSound);
		}

		UAISense_Hearing::ReportNoiseEvent(World, MuzzleLocation, 1.0f, UGameplayStatics::GetPlayerCharacter(World, 0), 0.0f, "Shot");

		if (MuzzleFlashParticles)
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleFlashParticles, mWeaponMesh, MuzzleSocketName);
		}

		if (FireAnimation != NULL)
		{
			if (UAnimInstance* AnimInstance = mWeaponMesh->GetAnimInstance())
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}

		// Firing our bullets
		if (ADGGameMode* GameMode = Cast<ADGGameMode>(UGameplayStatics::GetGameMode(World)))
		{
			if (ADGCharacter* PlayerCharacter = Cast<ADGCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0)))
			{
				APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);

				FVector CameraAimingPoint = PlayerCharacter->GetCameraAimingPoint();
				FVector AimingDirection = (CameraAimingPoint - MuzzleLocation).GetSafeNormal();
				float SpreadHalfAngleRadians = FMath::DegreesToRadians(BulletsSpreadAngle);

				static const float BulletTraceDistance = GetRange();
				
				for (int32 i = 0; i < BulletsCount; ++i)
				{
					FVector ConeVector = FMath::VRandCone(AimingDirection, SpreadHalfAngleRadians);
					FVector TraceEnd = MuzzleLocation + ConeVector * BulletTraceDistance;
			
					GameMode->FireBullet(MuzzleLocation, TraceEnd, BulletDamage, PlayerController, PlayerCharacter, UDGDamageTypeShot::StaticClass(), this);
				}
			}
		}
	}
}

void ADGWeaponShotgun::EndFire()
{
	Super::EndFire();
}