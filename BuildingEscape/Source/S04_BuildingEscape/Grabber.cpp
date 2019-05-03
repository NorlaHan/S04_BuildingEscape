// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();	
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// Bad practice start###########
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector()*Reach);
	/// Bad Practice end############

	/// if the pgysicshandwl is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		/// move the object that we're holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}		
}

/// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent() 
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		/// Physics handle is found
		UE_LOG(LogTemp, Warning, TEXT("%s Congrats. PhysicsHandle found"), *GetOwner()->GetName());
	}
	else
	{
		/// 
		UE_LOG(LogTemp, Error, TEXT("%s PhysicsHandle is missing!"), *GetOwner()->GetName());
	}
}

/// Look for Input component (only appears at run-time)
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Congrats. Input Component found"), *GetOwner()->GetName());
		/// Bind the input action
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s Input Component is missing!"), *GetOwner()->GetName());
	}
}


void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Grab pressed"), *GetOwner()->GetName());

	/// Line trace and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	/// If we hit something then attach a physics handle
	if (ActorHit)
	{
		// Attach physics handle
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			HitResult.Location,
			FRotator::ZeroRotator
		);
	}	
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Get player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector()*Reach);

	/// Draw a red trace in the world to visualise
	/*
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		1.f
	);
	*/

	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line-trace (AKA Ray-cast) out to reach distance
	FHitResult Hit;
	bool HitValid =
		GetWorld()->LineTraceSingleByObjectType(
			OUT Hit,
			PlayerViewPointLocation,
			LineTraceEnd,
			FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
			TraceParameters
		);
	/// See what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is in tracing."), *(ActorHit->GetName()));
	}
	return Hit;
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Grab Release"), *GetOwner()->GetName());
	// TODO release physics handle
	PhysicsHandle->ReleaseComponent();
}

