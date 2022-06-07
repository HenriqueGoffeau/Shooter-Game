// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearBalckboardValue.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearBalckboardValue::UBTTask_ClearBalckboardValue()
{
	NodeName = TEXT("Clear Blackboard Value");
}

EBTNodeResult::Type UBTTask_ClearBalckboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

	return EBTNodeResult::Succeeded;
}
