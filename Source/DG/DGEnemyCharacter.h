
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DGEnemyInterface.h"
#include "DGEnemyCharacter.generated.h"

UCLASS()
class DG_API ADGEnemyCharacter : public ACharacter, public IDGEnemyInterface
{
	GENERATED_BODY()

public:
	ADGEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Gameplay)
	void SetRunning(bool IsRunning);

	UFUNCTION(BlueprintPure)
	float GetSpeedNormalized() const;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Die_Implementation() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	float mWalkSpeed = 200;

	UPROPERTY(EditDefaultsOnly)
	float mRunSpeed = 500;

	UPROPERTY(EditDefaultsOnly)
	float mMaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
	float mHealth;

	bool mIsDead = false;
};
