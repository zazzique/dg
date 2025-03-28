// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DGWeapon.h"
#include "DGWeaponShotgun.generated.h"

UCLASS()
class DG_API ADGWeaponShotgun : public ADGWeapon
{
	GENERATED_BODY()
	
public:
	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
	UParticleSystem* MuzzleFlashParticles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	int32 BulletsCount = 8;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
	float BulletsSpreadAngle = 4.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
	float BulletDamage = 5.0f;

public:	
	ADGWeaponShotgun();

	virtual void BeginFire() override;
	virtual void EndFire() override;

private:
	int8 ActiveBarrel = 0;
};
