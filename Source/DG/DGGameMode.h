// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DGGameMode.generated.h"

USTRUCT(BlueprintType)
struct FDGPhysMatGameInfo
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = VFX)
	UParticleSystem* BulletHitParticles;
};

UCLASS(minimalapi)
class ADGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
	TMap<TEnumAsByte<EPhysicalSurface>, FDGPhysMatGameInfo> PhysMatInfos;

public:
	ADGGameMode();

	bool FireBullet(const FVector& Start, const FVector& End, float Damage, AController* InstigatorController, AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass, AActor* Weapon = nullptr);
};



