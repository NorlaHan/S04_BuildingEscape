// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
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

	/*// Bad practice start###########
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector()*Reach);
	*/// Bad Practice end############

	if (!PhysicsHandle)	{return;}

	/// if the PhysicsHandle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		/// move the object that we're holding
		LineTracingPointLocation LTPLocation = GetLineTracingPoint();	// Call LTP struct
		PhysicsHandle->SetTargetLocation(LTPLocation.EndPointLocation);
	}		
}

/// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent() 
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s PhysicsHandle component is missing!"), *GetOwner()->GetName());
	}
}

/// Look for Input component (only appears at run-time)
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
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
	// UE_LOG(LogTemp, Warning, TEXT("%s Grab pressed"), *GetOwner()->GetName());

	/// Line trace and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();		// FHitResult
	auto ComponentToGrab = HitResult.GetComponent();	// *UPrimitiveComponent, get the mesh in this case
	auto ActorHit = HitResult.GetActor();				// *AActor

	/// If we hit something then attach a physics handle
	if (ActorHit)
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,		// *UPrimitiveComponent
			NAME_None,				// FName BoneName, not bone needed in this case
			//ComponentToGrab->GetOwner()->GetActorLocation()	// Grab by the root.
			HitResult.Location		// FVector Location			// Grab by the point where Line tracer hits.
			,FRotator::ZeroRotator	// Rotator Rotation
		);
	}	
}

void UGrabber::Release()
{
	// UE_LOG(LogTemp, Warning, TEXT("%s Grab Release"), *GetOwner()->GetName());
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

// Return HitResult if the Line tracing hits.
FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	LineTracingPointLocation LTPLocation = GetLineTracingPoint();	// Call LTP struct

	/// Draw a red trace in the world to visualise
	/*
	DrawDebugLine(
		GetWorld(),
		LTPLocation.StartPointLocation,
		LTPLocation.EndPointLocation,
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
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType
(
		OUT HitResult,
		LTPLocation.StartPointLocation,
		LTPLocation.EndPointLocation,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	/// See what we hit
	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is in tracing."), *(ActorHit->GetName()));
	}
	return HitResult;
}

// Get the Line-Tracing Start and End point in a struct
LineTracingPointLocation UGrabber::GetLineTracingPoint() const
{
	LineTracingPointLocation LineTracingPoint;
	// Get player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector()*Reach);

	LineTracingPoint.StartPointLocation = PlayerViewPointLocation;
	LineTracingPoint.EndPointLocation = LineTraceEnd;
	//UE_LOG(LogTemp,Warning,TEXT("LineTracingPointLocation called!"));
	return LineTracingPoint;
}
