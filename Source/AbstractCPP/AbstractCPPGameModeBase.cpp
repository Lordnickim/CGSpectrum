// Copyright Epic Games, Inc. All Rights Reserved.


#include "AbstractCPPGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void AAbstractCPPGameModeBase::StartPlay() {
	Super::StartPlay();
	if (ObjectiveWidget == nullptr) {
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		ObjectiveWidget = CreateWidget<UUserWidget>(PlayerController, ObjectiveWidgetClass);
	}
	if (ObjectiveWidget) {
		ObjectiveWidget->AddToViewport();
	}
}

