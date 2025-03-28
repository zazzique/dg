
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DGSoldierAIController.generated.h"

UCLASS()
class DG_API ADGSoldierAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
};
