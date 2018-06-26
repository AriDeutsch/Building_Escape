// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
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

	Owner = GetOwner();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	//Check if time to close door
	else if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime >= DoorCloseDelay)
	{
		CloseDoor();
	}
	
}


void UOpenDoor::OpenDoor()
{

	z = sinf((SwingAngle*2*PI/360.f) / 2.f);
	w = cosf((SwingAngle*2*PI/360.f) / 2.f);
	Owner->SetActorRotation(FQuat(0.f,0.f,z,w));
	//Owner->SetActorRotation(FRotator(0.f, SwingAngle, 0.f));
	
}


//Changes the sign of the angle of the OpenDoor function
void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FQuat(0.f, 0.f, 0.f, 1.f));
	//Or I can use: Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}