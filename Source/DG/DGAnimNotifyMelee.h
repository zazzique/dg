
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DGAnimNotifyMelee.generated.h"

UCLASS()
class DG_API UDGAnimNotifyMelee : public UAnimNotify
{
	GENERATED_BODY()

public:
	UDGAnimNotifyMelee();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	// Base melee damage
	UPROPERTY(EditAnywhere, Category="AnimNotify")
	float DamageAmount = 20.0f;

	// Radius of melee damage
	UPROPERTY(EditAnywhere, Category="AnimNotify")
	float DamageRadius = 100.0f;

	// Socket name where radial damage will be located
	UPROPERTY(EditAnywhere, Category="AnimNotify")
	FName SocketName;
};
