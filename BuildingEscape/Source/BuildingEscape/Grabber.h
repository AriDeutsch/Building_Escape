// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//Length of line trace
	UPROPERTY(EditAnywhere)float Reach = 200.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UInputComponent* InputComponent = nullptr;

	AActor* GrabbedObject = nullptr;

	struct TraceInfo
	{
		FVector StartPoint;
		FVector EndPoint;
	};

	void FindPhysicsHandleComponent();

	void FindAndSortInput();

	const TraceInfo GetLineTrace() const;

	AActor* GetFirstPhysicsBodyInReach() const;

	void Grab();

	void Release();
};
