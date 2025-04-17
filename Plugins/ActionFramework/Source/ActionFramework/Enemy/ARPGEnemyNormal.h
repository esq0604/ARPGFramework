// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/Enemy/ARPGEnemy.h"
#include "ARPGEnemyNormal.generated.h"

/**
 * 
 */
//체력, 체간을 ui와 바인딩한다.
//데미지를 입는다 -> widgetcomponent visibility를 활성화한다
//데미지를 입은 상태에서 n 초 있으면 visibility를 비활성화한다.


class UWidgetComponent;

UCLASS()
class ACTIONFRAMEWORK_API AARPGEnemyNormal : public AARPGEnemy
{
	GENERATED_BODY()
	
public:
	AARPGEnemyNormal();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWidgetComponent> EnemyStatusBar;
};
