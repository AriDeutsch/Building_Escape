// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Gameframework/Actor.h"

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

	FString ownerRot = GetOwner()->GetActorRotation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("Door angle is %s\n"), *ownerRot);

	

	FQuat _90DegRot = FQuat(0, 0, 1 / sqrt(2), 1 / sqrt(2)); //fheihdgdk

	GetOwner()->SetActorRelativeRotation(_90DegRot);

	ownerRot = GetOwner()->GetActorRotation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("Door angle is %s\n"), *ownerRot);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

