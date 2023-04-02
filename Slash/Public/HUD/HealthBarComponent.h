// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	void SetHealthPercet(float Percent);

private:
	UPROPERTY()
	TObjectPtr<class UHealthBar> HealthBarWidget;
	
};
