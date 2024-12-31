// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "TargetingComponent.generated.h"


/**
 *  Ÿ���ð��� ����� ��������������Ʈ�� �������� Ŭ�������� �����ϰ�.
 *  �ش� TargetingComponent�� �������ִ� ��ü�� �÷��̾ Lock�� �� �ֵ��� �����߽��ϴ�.
 *  SceneComponenet�� ����� ������ , �ش� ������Ʈ�� ��ġ�� ī�޶� ������Ű�� ���� ��ġ���� �޾ƿ� �� �ִ� ������Ʈ�� �θ� �����Ͽ����ϴ�.
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class ACTIONFRAMEWORK_API UTargetingComponent : public USphereComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetingComponent();

		
};
