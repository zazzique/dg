// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "DGGameMode.h"
#include "DGHUD.h"
#include "DGCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ADGGameMode::ADGGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/Framework/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ADGHUD::StaticClass();
}

bool ADGGameMode::FireBullet(const FVector& Start, const FVector& End, float Damage, AController* InstigatorController, AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass, AActor* Weapon)
{
	bool result = false;

	FCollisionQueryParams TraceParams;
	if (DamageCauser)
	{
		TraceParams.AddIgnoredActor(DamageCauser);
	}
	if (Weapon)
	{
		TraceParams.AddIgnoredActor(Weapon);
	}
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult HitResult;

	UWorld* World = GetWorld();
	if (World->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel2, TraceParams, FCollisionResponseParams::DefaultResponseParam))
	{
		const TEnumAsByte<EPhysicalSurface> SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());

		const FVector Direction = (End - Start).GetSafeNormal();

		if (const FDGPhysMatGameInfo* PhysMatGameInfo = PhysMatInfos.Find(SurfaceType))
		{
			if (PhysMatGameInfo->BulletHitParticles)
			{
				const FVector Reflection = FMath::GetReflectionVector(Direction, HitResult.ImpactNormal);
				const FRotator Rotation = Reflection.Rotation();
				UGameplayStatics::SpawnEmitterAtLocation(World, PhysMatGameInfo->BulletHitParticles, HitResult.ImpactPoint, Rotation);
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Red, false, 2.0f);
		}

		if (HitResult.GetActor())
		{
			const FPointDamageEvent DmgEvent = FPointDamageEvent(Damage, HitResult, Direction, DamageTypeClass);
			HitResult.GetActor()->TakeDamage(Damage, FDamageEvent(DamageTypeClass), InstigatorController, DamageCauser);
		}

		result = true;
	}

	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);

	return result;
}