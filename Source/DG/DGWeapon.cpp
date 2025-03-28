
#include "DGWeapon.h"

ADGWeapon::ADGWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	SetActorEnableCollision(false);

	mWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	//mWeaponMesh->SetOnlyOwnerSee(true);
	mWeaponMesh->SetupAttachment(RootComponent);
	mWeaponMesh->bCastDynamicShadow = false;
	mWeaponMesh->CastShadow = false;
}


void ADGWeapon::BeginPlay()
{
	Super::BeginPlay();

	SetActorRelativeLocation(RelativeOffsetLocation);
}

/*void ADGWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

void ADGWeapon::BeginFire()
{
}

void ADGWeapon::EndFire()
{
}

