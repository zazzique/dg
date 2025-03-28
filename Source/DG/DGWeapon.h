#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DGWeapon.generated.h"

UCLASS()
class DG_API ADGWeapon : public AActor
{
	GENERATED_BODY()

protected:
	/** Gun mesh for first person view */
	UPROPERTY(EditAnywhere, Category = Mesh)
	class USkeletalMeshComponent* mWeaponMesh;

public:
	/** Weapon offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FVector RelativeOffsetLocation;

	/** Camera FOV for first person view*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float CameraFov = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float FireRange = 1000.0f * 100.0f;
	
public:	
	ADGWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	//virtual void Tick(float DeltaTime) override;

	virtual void BeginFire();
	virtual void EndFire();

	float GetRange() const { return FireRange; }

};
