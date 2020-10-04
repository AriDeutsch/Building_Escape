// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "Engine/TriggerBox.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()
		
public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable)FDoorEvent OpenRequest;
	UPROPERTY(BlueprintAssignable)FDoorEvent CloseRequest;

	/*Rotation of door when fully opened*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)FRotator OpenedConfig = FRotator(0.f, 0.f, 0.f);

	/*Rotation of door when fully closed*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)FRotator ClosedConfig = FRotator(0.f, 0.f, 0.f);

	/*Which object will act as the door*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)AActor* Door = nullptr;

	UPROPERTY(EditAnywhere, BluePrintReadOnly)float DoorSwingTime = 1.f;
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
		
private:
	
	void OpenDoor();
	void CloseDoor();
	float GetTotalMassOnPlate();
	void ResetTriggeringActors();
	
	/*Overlap with required total mass will open the door*/
	AActor * PressurePlate = GetOwner();

	/*Overlap with pawn will move the relevant actor(s) out of pressure plate trigger, thus closing door*/
	UPROPERTY(EditAnywhere)ATriggerVolume * ResetTrigger = nullptr;

	/*Define where the relevant actor(s) will reset to after the respective door closes*/
	UPROPERTY(EditAnywhere)TArray<FVector> ResetLocations;

	UPROPERTY(EditAnywhere)float RequiredMass = 38.f;
};
