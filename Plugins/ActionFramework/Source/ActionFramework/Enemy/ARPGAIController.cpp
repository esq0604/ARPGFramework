// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/Enemy/ARPGAIController.h"
#include "ActionFramework/Enemy/ARPGEnemyBoss.h"
#include "ActionFramework/AbilitySystem/ARPGAbilitySystemLibrary.h"
#include "ActionFramework/UI/OverlayPresenter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "ActionFramework/SubSystem/BossManager.h"

AARPGAIController::AARPGAIController()
{
	Blackboard= CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(Blackboard);
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	check(BehaviorTreeComponent);
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComponent");

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("AISense_Sight");
	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = 1000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 70.0f;
	
	AIPerceptionComponent->ConfigureSense(*SightConfig);

	SetPerceptionComponent(*AIPerceptionComponent);
}

void AARPGAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AARPGAIController::OnPerceptionUpdated);
	}
}

void AARPGAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	//보스인지 판단하는 방법 ->IsA
	//태그 (Enemy.Type.Boss) 및 데이터에셋?
	//인터페이스, IBossable? 
	//일단 1번으로 하고 나중에 뭔가 필요하면 2,3번으로 수정할듯.

	if (GetPawn()->IsA<AARPGEnemyBoss>())
	{
		UOverlayPresenter* Presenter = UARPGAbilitySystemLibrary::GetOverlayWidgetPresenter(this);
		if (Presenter)
		{
			Presenter->SetBoss(GetPawn());
		}
	}
}