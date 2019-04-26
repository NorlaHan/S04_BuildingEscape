// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
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

	// Find the owening actor
	AActor* Owner = GetOwner();
	FString OwnerRot = Owner->GetActorRotation().Vector().ToString();

	// Create a rotator
	UE_LOG(LogTemp, Warning, TEXT("%s rotation is %s"),*Owner->GetName(),*OwnerRot );
	FRotator NewRotation = FRotator(0.f, -60.f, 0.f);

	// Set the door rotation
	Owner->SetActorRotation(NewRotation);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

