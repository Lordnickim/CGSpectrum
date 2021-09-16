// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "AbstractCPPGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ABSTRACTCPP_API AAbstractCPPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	void StartPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ObjectiveWidgetClass;
	UUserWidget* ObjectiveWidget = nullptr;
};
