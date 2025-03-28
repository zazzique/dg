
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DGEnemyCharacter.h"
#include "DGSoldierCharacter.generated.h"

UCLASS()
class DG_API ADGSoldierCharacter : public ADGEnemyCharacter
{
	GENERATED_BODY()

public:
	ADGSoldierCharacter();

protected:
	virtual void BeginPlay() override;

};
