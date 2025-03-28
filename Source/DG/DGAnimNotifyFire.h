
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DGAnimNotifyFire.generated.h"

UCLASS()
class DG_API UDGAnimNotifyFire : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UDGAnimNotifyFire();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	// Bullet damage
	UPROPERTY(EditAnywhere, Category="AnimNotify")
	float BulletDamage = 20.0f;

	// Bullet trace distance
	UPROPERTY(EditAnywhere, Category="AnimNotify")
	float BulletTraceDistance = 1000.0f * 100.0f;
};
