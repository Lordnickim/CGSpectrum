// Copyright Epic Games, Inc. All Rights Reserved.


#include "AbstractCPPGameModeBase.h"
#include "ObjectiveWorldSubsystem.h"

void AAbstractCPPGameModeBase::StartPlay() {
	Super::StartPlay();
	UObjectiveWorldSubsystem* ObjectiveWorldSubsystem = GetWorld()->GetSubsystem<UObjectiveWorldSubsystem>();
	if (ObjectiveWorldSubsystem) {
		ObjectiveWorldSubsystem->CreateObjectiveWidget(ObjectiveWidgetClass);
		ObjectiveWorldSubsystem->DisplayObjectiveWidget();
	}
}

