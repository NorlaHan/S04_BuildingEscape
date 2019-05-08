// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "GameFrameWork/Actor.h"

#define OUT	// Macro, do nothing but as a mark

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ....
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();	
	Owner = GetOwner();	// Find the owening actor
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s, PressurePlate is nullptr please assign valid object."), *Owner->GetName());
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// poll the trigger volume
		// If the ActorThatOpens is in the volume.
		//if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	if (GetTotalMassOfActorsOnPlate() > TriggerMass)	// TODO make into a parameter
	{
		//UE_LOG(LogTemp, Warning, TEXT("Something Enters, %s open."), *GetOwner()->GetName());
		OnOpen.Broadcast();
		//LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	else
	{
		OnClose.Broadcast();
	}

	// Check if it is time to close the door.
	//if (GetWorld()->GetTimeSeconds() > (LastDoorOpenTime + DoorCloseDelay))
	//{
	//	CloseDoor();
	//}
	
}

/*
void UOpenDoor::OpenDoor()
{	
	//FString OwnerRot = Owner->GetActorRotation().Vector().ToString();
	//UE_LOG(LogTemp, Warning, TEXT("%s rotation is %s"), *Owner->GetName(), *OwnerRot);

	//Owner->SetActorRotation(FRotator(0.f, OpenAlgle, 0.f));	// Set the door rotation
	OnOpen.Broadcast();
}


void UOpenDoor::CloseDoor()
{
	FString OwnerRot = Owner->GetActorRotation().Vector().ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s rotation is %s"), *Owner->GetName(), *OwnerRot);

	//Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));	// Set the door rotation
	OnClose.Broadcast();
}
*/

float UOpenDoor::GetTotalMassOfActorsOnPlate() 
{
	float TotalMass = 0.f;

	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate)	{return TotalMass;}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		
		UE_LOG(LogTemp, Warning, TEXT(" %s overlapped, weight is %s"),*Actor->GetName(),*FString::SanitizeFloat(TotalMass));
	}
	// Iterate through them adding their masses

	return TotalMass;
}
