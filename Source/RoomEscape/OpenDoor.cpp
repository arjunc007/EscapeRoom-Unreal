// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine//World.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	StartRotation = Owner->GetActorRotation();

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor()
{
	//Create a Rotator
	FRotator NewRotation = StartRotation + FRotator(0.0f, OpenAngle, 0.0f);

	//Set the Door Rotation
	Owner->SetActorRelativeRotation(NewRotation);
}

void UOpenDoor::CloseDoor()
{
	//Set the Door Rotation
	Owner->SetActorRelativeRotation(StartRotation);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	//Check if time to close the door
	if ((GetWorld()->GetTimeSeconds() - LastDoorOpenTime) > DoorCloseDelay)
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0;
	TArray<AActor*> OverlappingActors;

	if(PressurePlate)
		PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	UE_LOG(LogTemp, Warning, TEXT("Number of Actors: %d"), OverlappingActors.Num())

	for (const auto& Actor : OverlappingActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *Actor->GetName())
		TotalMass += static_cast<UPrimitiveComponent*>(Actor->GetComponentByClass(UPrimitiveComponent::StaticClass()))->GetMass();
	}

	return TotalMass;
}