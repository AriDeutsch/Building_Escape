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

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!PressurePlate)UE_LOG(LogTemp, Error, TEXT("No Pressure Plate initialised"));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetTotalMassOnPlate() > RequiredMass)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
		if (ResetTrigger->IsOverlappingActor(GetWorld()->GetFirstPlayerController()->GetPawn()))ResetDoorTrigger();
	}
	else CloseDoor();
}

float UOpenDoor::GetTotalMassOnPlate()
{
	float mass = 0.f;
	TArray<AActor*> theActors;
	if (!PressurePlate) {return mass;}
	PressurePlate->GetOverlappingActors(theActors);
	for (int32 i = 0; i < theActors.Num(); ++i)
	{
		///UActorComponent* comp = theActors[i]->FindComponentByClass<UPrimitiveComponent>();
		UActorComponent* comp = theActors[i]->GetComponentByClass(UPrimitiveComponent::StaticClass());
		UPrimitiveComponent* comp1 = Cast<UPrimitiveComponent>(comp);
		mass += comp1->GetMass();
	}
	return mass;
}



void UOpenDoor::ResetDoorTrigger()
{
	TArray<AActor*> theActors;
	if (PressurePlate)PressurePlate->GetOverlappingActors(theActors);
	for (int32 i = 0; i < theActors.Num(); ++i)
	{
		theActors[i]->SetActorLocation(ResetLocations[i]);
	}
}


void UOpenDoor::OpenDoor()
{
	///z = sinf((SwingAngle*2*PI/360.f) / 2.f);
	///w = cosf((SwingAngle*2*PI/360.f) / 2.f);
	///Owner->SetActorRotation(FQuat(0.f,0.f,z,w));
	//Owner->SetActorRotation(OpenedConfig);
	OpenRequest.Broadcast();
}


//Changes the sign of the angle of the OpenDoor function
void UOpenDoor::CloseDoor()
{
	///Owner->SetActorRotation(FQuat(0.f, 0.f, 0.f, 1.f));
	//Owner->SetActorRotation(ClosedConfig);
	CloseRequest.Broadcast();
}