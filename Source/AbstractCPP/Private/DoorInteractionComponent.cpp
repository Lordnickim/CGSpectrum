// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorInteractionComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/TriggerBox.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "ObjectiveComponent.h"

constexpr float FLT_METERS(float meters) { return meters * 100.f; }

static TAutoConsoleVariable<bool> CVarToggleDebugDoor(
	TEXT("Abstraction.DoorInteractionComponent.Debug"),
	false,
	TEXT("Toggle DoorInteractionComponent debug display."),
	ECVF_Default
);

// Sets default values for this component's properties
UDoorInteractionComponent::UDoorInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DoorState = EDoorState::DS_Closed;

	CVarToggleDebugDoor.AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateStatic(&UDoorInteractionComponent::OnDebugToggled));

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
	
	
}


// Called every frame
void UDoorInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FRotator CurrentRoation = GetOwner()->GetActorRotation();
	if (DoorState == EDoorState::DS_Closed) {
		if (TriggerBox && GetWorld() && GetWorld()->GetFirstLocalPlayerFromController()) {
			APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
			if (PlayerPawn && TriggerBox->IsOverlappingActor(PlayerPawn)) {

				DoorState = EDoorState::DS_Opening;
				CurrentRotationTime = 0.f;



			}
		}
	}
	else if (DoorState == EDoorState::DS_Opening) {
		CurrentRotationTime += DeltaTime;
		//Linear Rotation or Constant time
		//const float RotationAlpha = FMath::Clamp(CurrentRotationTime / TimeToRotate, 0.f, 1.0f);

		//Curve Rotation or Ease Time 
		const float TimeRatio = FMath::Clamp(CurrentRotationTime / TimeToRotate, 0.f, 1.0f);
		const float RotationAlpha = OpenCurve.GetRichCurveConst()->Eval(TimeRatio);
		const FRotator CurrentRotation = FMath::Lerp(StartRotation, FinalRotation, RotationAlpha);
		GetOwner()->SetActorRotation(CurrentRotation);
		if (TimeRatio >= 1.f) {
			OnDoorOpen();
		}
			
	}
	DebugDraw();
}

void UDoorInteractionComponent::OnDoorOpen() {
	DoorState = EDoorState::DS_Open;
	UObjectiveComponent* ObjectiveComponent = GetOwner()->FindComponentByClass<UObjectiveComponent>();
	if (ObjectiveComponent) {
		ObjectiveComponent->SetState(EObjectiveState::OS_Completed);
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("DoorOpenedd"));
}

void UDoorInteractionComponent::OnDebugToggled(IConsoleVariable* Var) {
	UE_LOG(LogTemp, Warning, TEXT("OnDebugToggled"));
}

void UDoorInteractionComponent::DebugDraw() {
	if (CVarToggleDebugDoor->GetBool()) {
		FVector Offset(FLT_METERS(-0.75f), 0.f, FLT_METERS(2.5f));
		FVector StartLocation = GetOwner()->GetActorLocation() + Offset;
		FString EnumAsString = TEXT("Door State: ") + UEnum::GetDisplayValueAsText(DoorState).ToString();
		DrawDebugString(GetWorld(), Offset, EnumAsString, GetOwner(), FColor::Blue, 0.f);
	}
}