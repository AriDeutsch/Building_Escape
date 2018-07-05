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
	// check if the physics handle component is attached
	if (PhysicsHandle->GetGrabbedComponent()) 
	{
		TraceInfo CurrentTrace = GetLineTraceCoordinates();
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
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else{UE_LOG(LogTemp, Error, TEXT("%s actor has no input component attached"), *GetOwner()->GetName())}
}

const   UGrabber::TraceInfo UGrabber::GetLineTraceCoordinates() const
{
	///Initialise struct instance
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

	TraceInfo LineTrace = GetLineTraceCoordinates();
	GetWorld()->LineTraceSingleByObjectType(
		RETURN_RESULT hitResult,
		LineTrace.StartPoint,
		LineTrace.EndPoint,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
	);
	return hitResult;
}

void UGrabber::Grab()
{
	///We need to get the primitive component(defines geometry of actor) of the actor hit by line trace, as well as the actor itself
	UPrimitiveComponent* GrabbedComponent = GetFirstPhysicsBodyInReach().GetComponent();
	if (GrabbedComponent)
	{
		AActor* GrabbedActor = GrabbedComponent->GetOwner();
		if (PhysicsHandle)PhysicsHandle->GrabComponentAtLocationWithRotation(GrabbedComponent, NAME_None, GrabbedActor->GetActorLocation(), GrabbedActor->GetActorRotation());
		UE_LOG(LogTemp, Warning, TEXT("%s component grabbed"), *GrabbedActor->GetName())
	}
}

void UGrabber::Release()
{
	if (PhysicsHandle)PhysicsHandle->ReleaseComponent();
}