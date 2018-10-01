// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "Engine/Public/DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));

	///Look for attached physics handle
	PhysicsHandle = static_cast<UPhysicsHandleComponent*>(GetOwner()->GetComponentByClass(UPhysicsHandleComponent::StaticClass()));
	
	if (PhysicsHandle)
	{
		//Physics handle found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Physics handle not found on %s"), *(GetOwner()->GetName()))
	}
	
	InputComponent = static_cast<UInputComponent*>(GetOwner()->GetComponentByClass(UInputComponent::StaticClass()));

	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input component found!"))
		///Bind input action
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input component not found on %s"), *GetOwner()->GetName())
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"))
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Object Released"));
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Get player viewpoint
	FVector PlayerLocation;
	FRotator PlayerRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);

	FVector LineTraceEnd = PlayerLocation + PlayerRotation.Vector() * Reach;

	DrawDebugLine(GetWorld(), 
		PlayerLocation, 
		LineTraceEnd, 
		FColor::Red, 
		false, 
		0.f, 
		0.f, 
		10.f
	);

	///Setup query params
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	///Line-trace (aka Raycast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(OUT Hit,
		PlayerLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	///See what is hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s"), *(ActorHit->GetName()))
	}
}

