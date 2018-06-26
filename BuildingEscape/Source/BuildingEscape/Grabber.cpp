// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

#define RETURN_RESULT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	FindAndSortInput();
	
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the physics handle component is attached
	// move grabbed object every frame
}


void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) { UE_LOG(LogTemp, Error, TEXT("%s actor has no physics handle component attached"), *GetOwner()->GetName()) }
}

void UGrabber::FindAndSortInput()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s actor has an input component"), *GetOwner()->GetName())
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s actor has no input component attached"), *GetOwner()->GetName())
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	/// Declare view point variables
	FVector playerViewLoc;
	FRotator playerViewRot;

	//return variable
	FHitResult hitResult; 

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		RETURN_RESULT playerViewLoc,
		RETURN_RESULT playerViewRot
	);

	FVector LineTraceEndPoint = playerViewLoc + Reach * playerViewRot.Vector();

	/// Ray-cast out to 'LineTraceEndPoint' on every tick
	GetWorld()->LineTraceSingleByObjectType(
		RETURN_RESULT hitResult,
		playerViewLoc,
		LineTraceEndPoint,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
	);
	return hitResult;
}

void UGrabber::Grab()
{
	FHitResult GrabbedObject = GetFirstPhysicsBodyInReach();
	if (GrabbedObject.GetActor())
	{
		///...Attach
		UE_LOG(LogTemp, Warning, TEXT("%s grabbed"), *GrabbedObject.GetActor()->GetName())
	}
	else{UE_LOG(LogTemp, Warning, TEXT("No object within reach"))}
}

void UGrabber::Release()
{
	FHitResult GrabbedObject = GetFirstPhysicsBodyInReach();
	if (GrabbedObject.GetActor())
	{
		///...Attach
		UE_LOG(LogTemp, Warning, TEXT("%s releasd"), *GrabbedObject.GetActor()->GetName())
	}
}