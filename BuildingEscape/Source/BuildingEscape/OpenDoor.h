// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
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
	
	UPROPERTY(EditAnywhere)float SwingAngle = 90.f;
	
	/*
	Quaternion inputs definition:
	SwingAngle is in radians
	x = RotationAxis.x * sin(SwingAngle / 2)
	y = RotationAxis.y * sin(SwingAngle / 2)
	z = RotationAxis.z * sin(SwingAngle / 2)
	w = cos(SwingAngle / 2)
	*/
	float z;
	float w;

	UPROPERTY(EditAnywhere)ATriggerVolume * PressurePlate;

	UPROPERTY(EditAnywhere)float DoorCloseDelay = 1.f;

	UPROPERTY(EditAnywhere)bool IsOwnerGrabbable;

	float LastDoorOpenTime;

	AActor* ActorThatOpens;

	AActor* Owner;
};
