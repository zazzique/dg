
#include "DGAnimNotifyFire.h"
#include "Kismet/GameplayStatics.h"
#include "DGGameMode.h"
//#include "DGCharacter.h"
#include "DGDamageTypes.h"

UDGAnimNotifyFire::UDGAnimNotifyFire()
	: Super()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(255, 0, 255, 255);
#endif // WITH_EDITORONLY_DATA
}

void UDGAnimNotifyFire::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		if (UWorld* World = MeshComp->GetWorld())
		{
			if (ADGGameMode* GameMode = Cast<ADGGameMode>(UGameplayStatics::GetGameMode(World)))
			{
				if (AActor* OwnerActor = MeshComp->GetOwner())
				{
					if (AController* InstigatorController = OwnerActor->GetInstigatorController())
					{
						FVector ControllerDir = InstigatorController->GetControlRotation().Vector();
						FVector TraceStart = InstigatorController->GetPawn()->GetActorLocation();
						FVector TraceEnd = TraceStart + ControllerDir * BulletTraceDistance;
			
						GameMode->FireBullet(TraceStart, TraceEnd, BulletDamage, InstigatorController, OwnerActor, UDGDamageTypeBullet::StaticClass());
					}
				}
			}
		}
	}
}