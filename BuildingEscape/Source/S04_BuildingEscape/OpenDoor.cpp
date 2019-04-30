// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFrameWork/Actor.h"

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
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// poll the trigger volume
	// If the ActorThatOpens is in the volume.
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Enters, %s open."), *ActorThatOpens->GetName(), *GetOwner()->GetName());
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() > (LastDoorOpenTime + DoorCloseDelay))
	{
		CloseDoor();
	}
	// Check if it is time to close the door.
}


void UOpenDoor::OpenDoor()
{	
	FString OwnerRot = Owner->GetActorRotation().Vector().ToString();
	//UE_LOG(LogTemp, Warning, TEXT("%s rotation is %s"), *Owner->GetName(), *OwnerRot);

	Owner->SetActorRotation(FRotator(0.f, OpenAlgle, 0.f));	// Set the door rotation
}


void UOpenDoor::CloseDoor()
{
	FString OwnerRot = Owner->GetActorRotation().Vector().ToString();
	//UE_LOG(LogTemp, Warning, TEXT("%s rotation is %s"), *Owner->GetName(), *OwnerRot);

	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));	// Set the door rotation
}


