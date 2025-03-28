
#include "DGAnimNotifyMelee.h"
#include "Kismet/GameplayStatics.h"
#include "DGDamageTypes.h"

UDGAnimNotifyMelee::UDGAnimNotifyMelee()
	: Super()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(255, 0, 255, 255);
#endif // WITH_EDITORONLY_DATA
}

void UDGAnimNotifyMelee::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		if (UWorld* World = MeshComp->GetWorld())
		{
			if (AActor* OwnerActor = MeshComp->GetOwner())
			{
				FVector SocketLocation = MeshComp->GetSocketLocation(SocketName);
				AController* InstigatorController = OwnerActor->GetInstigatorController();
				TArray<AActor*> IgnoredActors;
				IgnoredActors.Add(OwnerActor);

				UGameplayStatics::ApplyRadialDamage(World, DamageAmount, SocketLocation, DamageRadius, UDGDamageTypeMelee::StaticClass(), IgnoredActors, OwnerActor, InstigatorController, true, ECollisionChannel::ECC_GameTraceChannel2);
			}
		}
	}
}
