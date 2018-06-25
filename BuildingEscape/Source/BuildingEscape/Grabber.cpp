// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player view point this tick
	FVector playerViewLoc;
	FRotator playerViewRot;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerViewLoc, playerViewRot);

	//Log view point data on every tick
	/*UE_LOG(LogTemp, Warning, TEXT("Pawn is at %s, with rotation P=y, Y=z, R = x, %s"), 
		*playerViewLoc.ToString(),*playerViewRot.ToString());*/

	

	//Draw a red trace in the world to visualise
	FVector LineTraceEndPoint = playerViewLoc + Reach*playerViewRot.Vector();
	DrawDebugLine(
		GetWorld(), 
		playerViewLoc, 
		LineTraceEndPoint, 
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);

	// Ray-cast out to reach distance


	// See what we hit
}

