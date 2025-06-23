// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/Enemy/AI/Task/BTTask_ExecutePatternRow.h"
#include "ActionFramework/AbilitySystem/ARPGAttributeSet.h"
#include "ActionFramework/Enemy/AI/EnemyBehaviorPattern.h"

#include "GameplayAbilities/Public/AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTTask_ExecutePatternRow::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp)
	{
		return EBTNodeResult::Failed;
	}

	UObject* PatternDataTableObject = BBComp->GetValueAsObject(PatternDataTableKeySelector.SelectedKeyName);
	if (!PatternDataTableObject)
	{
		UE_LOG(LogTemp,Warning,TEXT("BehaviorTable is nullptr %s"), *FPaths::GetCleanFilename(__FILE__))
		return EBTNodeResult::Failed;
	}

	UObject* SelfActorObject = BBComp->GetValueAsObject(SelfActorKeySelector.SelectedKeyName);
	if (!SelfActorObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("SelfActorObject is nullptr %s"), *FPaths::GetCleanFilename(__FILE__))
		return EBTNodeResult::Failed;
	}

	IAbilitySystemInterface* ASCI = Cast<IAbilitySystemInterface>(SelfActorObject);
	if (!ASCI)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASCI is nullptr %s"), *FPaths::GetCleanFilename(__FILE__))

		return EBTNodeResult::Failed;
	}

	UAbilitySystemComponent* ASC = ASCI->GetAbilitySystemComponent();
	if (!ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASC is nullptr %s"), *FPaths::GetCleanFilename(__FILE__))
		return EBTNodeResult::Failed;
	}

	UDataTable* PatternDataTable = Cast<UDataTable>(PatternDataTableObject);
	if (!PatternDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("PatternDataTable Cast Fail %s"), *FPaths::GetCleanFilename(__FILE__))
		return EBTNodeResult::Failed;
	}


	FGameplayTagContainer ExecutedAbilityTag;

	// 후보 패턴 Row 필터링
	TArray<const FEnemyBehaviorPattern*> CandidateRows;
	for (const auto& RowPair : PatternDataTable->GetRowMap())
	{
		FName RowName = RowPair.Key;
		const FEnemyBehaviorPattern* Pattern = reinterpret_cast<FEnemyBehaviorPattern*>(RowPair.Value);
		//if (!Pattern) continue;

		//// EnemyTag 검사
		//if (Pattern->EnemyTag != EnemyTag) continue;

		//// 체력 조건
		const UAttributeSet* Attr = ASC->GetAttributeSet(UARPGAttributeSet::StaticClass());
		const UARPGAttributeSet* ARPGAttr = Cast<UARPGAttributeSet>(Attr);
		float CurrentHealthPercent = ARPGAttr->GetHealth() / ARPGAttr->GetMaxHealth();
		if (CurrentHealthPercent > Pattern->HealthPercentThreshold) continue;

		//// Once 체크
		//if (Pattern->Once)
		//{
		//	FString UsedKeyStr = FString::Printf(TEXT("Used_%s"), *RowName.ToString());
		//	FName UsedKey = FName(*UsedKeyStr);
		//	if (BBComp->GetValueAsBool(UsedKey))
		//		continue; // 이미 사용한 패턴
		//}

		CandidateRows.Add(Pattern);
	}

	const FEnemyBehaviorPattern* SelectedPattern = CandidateRows[FMath::RandHelper(CandidateRows.Num())];
	UE_LOG(LogTemp, Warning, TEXT("SelectedPattern Ability Name : %s"), *SelectedPattern->AbilityTag.ToString());
	ExecutedAbilityTag.AddTag(SelectedPattern->AbilityTag);
	if (ASC->TryActivateAbilitiesByTag(ExecutedAbilityTag))
	{
		return  EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;

}
