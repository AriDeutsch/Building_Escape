// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "Gameframework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	if (!Door)UE_LOG(LogTemp, Warning, TEXT("No door associated with opening trigger %s"), *PressurePlate->GetName())
	if (!ResetTrigger)UE_LOG(LogTemp, Warning, TEXT("No Reset trigger associated with opening trigger %s"), *PressurePlate->GetName())
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetTotalMassOnPlate() > RequiredMass)
	{
		OpenDoor();

		///If the pawn has moved through doorway, it will hit the reset trigger, 
		///and the following line will remove any actors from the pressure plate
		///This is done to close the doors.
		if(ResetTrigger)
		{
			if (ResetTrigger->IsOverlappingActor(GetWorld()->GetFirstPlayerController()->GetPawn()))
			{
				ResetTriggeringActors();
			}
		}
	}
	else CloseDoor();
}

float UOpenDoor::GetTotalMassOnPlate()
{
	float mass = 0.f;
	TArray<AActor*> theActors;
	PressurePlate->GetOverlappingActors(theActors);
	for (int32 i = 0; i < theActors.Num(); ++i)
	{
		UPrimitiveComponent* comp = theActors[i]->FindComponentByClass<UPrimitiveComponent>();
		mass += comp->GetMass();
	}
	return mass;
}

void UOpenDoor::ResetTriggeringActors()
{
	TArray<AActor*> theActors;
	PressurePlate->GetOverlappingActors(theActors);
	for (int32 i = 0; i < theActors.Num(); ++i)
	{
		theActors[i]->SetActorLocation(ResetLocations[i]);
	}
}

void UOpenDoor::OpenDoor()
{
	//if(Door)OpenRequest.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	//if(Door)CloseRequest.Broadcast();
}