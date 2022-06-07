// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ClearBalckboardValue.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UBTTask_ClearBalckboardValue : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UBTTask_ClearBalckboardValue();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
