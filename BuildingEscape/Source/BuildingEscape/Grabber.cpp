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

	/// if the physics handle component is attached
	/// move grabbed object every frame
	if (PhysicsHandle->GetGrabbedComponent()) 
	{
		TraceInfo CurrentTrace = GetLineTrace();
		PhysicsHandle->SetTargetLocationAndRotation(CurrentTrace.EndPoint, CurrentTrace.Rotation);
	}

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

const   UGrabber::TraceInfo UGrabber::GetLineTrace() const
{
	///Initialise variables
	TraceInfo Trace{ FVector(0.f,0.f,0.f), FVector(0.f,0.f,0.f), FRotator(0.f,0.f,0.f) };
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		RETURN_RESULT Trace.StartPoint,
		RETURN_RESULT Trace.Rotation
	);

	Trace.EndPoint = Trace.StartPoint + Reach * Trace.Rotation.Vector();

	return Trace;
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	//return variable
	FHitResult hitResult;

	/// Ray-cast out to 'LineTraceEndPoint' on every tick
	GetWorld()->LineTraceSingleByObjectType(
		RETURN_RESULT hitResult,
		GetLineTrace().StartPoint,
		GetLineTrace().EndPoint,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
	);
	return hitResult;
}

void UGrabber::Grab()
{
	UPrimitiveComponent* GrabbedComponent = GetFirstPhysicsBodyInReach().GetComponent();
	AActor* GrabbedObject = GetFirstPhysicsBodyInReach().GetActor();
	if (GrabbedObject)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(GrabbedComponent, NAME_None, GrabbedObject->GetActorLocation(),GrabbedObject->GetActorRotation());

		UE_LOG(LogTemp, Warning, TEXT("%s grabbed"), *GrabbedObject->GetName())
	}
	else{UE_LOG(LogTemp, Warning, TEXT("No object within reach"))}
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}