// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Engine/TriggerCapsule.h"
#include "Engine/SpotLight.h"
#include "Runtime/Core/Public/Math/Color.h"
#include "StatuePuzzleLock.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorAccess);

USTRUCT() struct FLight_TriggerPair
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)ATriggerCapsule* theTrigger = nullptr;
	UPROPERTY(EditAnywhere)ASpotLight* theLight = nullptr;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UStatuePuzzleLock : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatuePuzzleLock();

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable)FDoorAccess OpenRequest;
	UPROPERTY(BlueprintAssignable)FDoorAccess CloseRequest;

	/*Rotation of door when fully opened*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)FRotator OpenedConfig = FRotator(0.f, 0.f, 0.f);

	/*Rotation of door when fully closed*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)FRotator ClosedConfig = FRotator(0.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, BluePrintReadOnly)float DoorOpenSwingTime = 4.f;
	UPROPERTY(EditAnywhere, BluePrintReadOnly)float DoorCloseSwingTime = 0.5f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	

private:
	/*Overlap with pawn will change statue permutation, thus closing door*/
	UPROPERTY(EditAnywhere)ATriggerVolume * ResetTrigger = nullptr;

	/*Insert all statues, including multiples with the same material*/
	UPROPERTY(EditAnywhere)TArray<AActor*> AllStatues;

	/*Insert each trigger that will overlap with a selected statue, and it's corresponding light*/
	UPROPERTY(EditAnywhere)TArray<FLight_TriggerPair> LightsAndTriggers;

	TArray<AActor*> StatuesToGuess;

	TArray<FVector> ResetLocations;
	TArray<FRotator> ResetRotations;

	void LightColourEvaluation();

	void ResetPuzzle();

	void CreateNewPermutation();

	///Green means right colour statue, right place
	void SetLightGreen(ASpotLight* light);
	///Yellow means right colour statue, wrong place
	void SetLightYellow(ASpotLight* light);
	///Red means wrong colour statue
	void SetLightRed(ASpotLight* light);
};
