// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class S04_BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor();

	void CloseDoor();

	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(VisibleAnywhere)	// Like a serializefield in Unity
	float OpenAlgle = 90.f;

	UPROPERTY(EditAnywhere)		// Like a public in Unity
	ATriggerVolume* PressurePlate;

	AActor* Owner;
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.f;
	float LastDoorOpenTime;

	// Return total mass in KG
	float GetTotalMassOfActorsOnPlate();

};
