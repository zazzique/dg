
#include "DGEnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/CollisionProfile.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "BrainComponent.h"

ADGEnemyCharacter::ADGEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADGEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	mHealth = mMaxHealth;
	mIsDead = false;

	SetRunning(false);
}

void ADGEnemyCharacter::SetRunning(bool IsRunning)
{
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->MaxWalkSpeed = IsRunning ? mRunSpeed : mWalkSpeed;
	}
}

float ADGEnemyCharacter::GetSpeedNormalized() const
{
	float Speed = GetVelocity().Size();

	if (Speed <= mWalkSpeed + 0.1f)
	{
		return UKismetMathLibrary::NormalizeToRange(Speed, 0.0f, mWalkSpeed) * 0.5f;
	}
	else
	{
		return 0.5f + UKismetMathLibrary::NormalizeToRange(Speed, 0.0f, mRunSpeed) * 0.5f;
	}
}

float ADGEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (IsDead())
	{
		return ActualDamage;
	}

	mHealth -= ActualDamage;

	if (mHealth <= 0.0f)
	{
		Execute_Die(this);
	}

	return ActualDamage;
}

void ADGEnemyCharacter::Die_Implementation()
{
	mIsDead = true;
	mHealth = 0.0f;

	GetCapsuleComponent()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->StopMovement();

		if (UBrainComponent* BrainComponent = AIController->GetBrainComponent())
		{
			BrainComponent->StopLogic("Death");
		}
	}
}

bool ADGEnemyCharacter::IsDead() const
{
	return mIsDead;
}
