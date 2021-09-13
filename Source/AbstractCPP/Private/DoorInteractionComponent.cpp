// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorInteractionComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/TriggerBox.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UDoorInteractionComponent::UDoorInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDoorInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	StartRotation = GetOwner()->GetActorRotation();
	FinalRotation = GetOwner()->GetActorRotation() + DesiredRotation;
	//ensure TimeToRoate is greater than EPSILON
	CurrentRotationTime = 0.f;
	// ...
	
}


// Called every frame
void UDoorInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FRotator CurrentRoation = GetOwner()->GetActorRotation();
	if (CurrentRotationTime < TimeToRotate) {
		if (TriggerBox && GetWorld() && GetWorld()->GetFirstLocalPlayerFromController()) {
			APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
			if (PlayerPawn && TriggerBox->IsOverlappingActor(PlayerPawn)) {
				CurrentRotationTime += DeltaTime;
				//Linear Rotation or Constant time
				//const float RotationAlpha = FMath::Clamp(CurrentRotationTime / TimeToRotate, 0.f, 1.0f);

				//Curve Rotation or Ease Time 
				const float TimeRatio = FMath::Clamp(CurrentRotationTime / TimeToRotate, 0.f, 1.0f);
				const float RotationAlpha = OpenCurve.GetRichCurveConst()->Eval(TimeRatio);
				const FRotator CurrentRotation = FMath::Lerp(StartRotation, FinalRotation, RotationAlpha);
				GetOwner()->SetActorRotation(CurrentRotation);

			}
		}
	}
}

