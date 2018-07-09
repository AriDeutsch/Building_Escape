// Fill out your copyright notice in the Description page of Project Settings.

#include "StatuePuzzleLock.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Materials/MaterialInterface.h"

// Sets default values for this component's properties
UStatuePuzzleLock::UStatuePuzzleLock()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UStatuePuzzleLock::BeginPlay()
{
	Super::BeginPlay();

	///When puzzle has been solved, all statues should return to original locations/rotations. Thus we must store those locations when play starts
	for (int i = 0; i < AllStatues.Num(); ++i)
	{
		ResetLocations.Add(AllStatues[i]->GetActorLocation());
		ResetRotations.Add(AllStatues[i]->GetActorRotation());
	}
	CreateNewPermutation();
}

// Called every frame
void UStatuePuzzleLock::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	LightColourEvaluation();
	if (ResetTrigger)
	{
		///We want to reset the puzzle if the player solves it and escapes. In that case, player will hit trigger
		if (ResetTrigger->IsOverlappingActor(GetWorld()->GetFirstPlayerController()->GetPawn()))
		{
			for (int i = 0; i < AllStatues.Num(); i++) 
			{
				//Only reset if it needs to be.
				if(!AllStatues[i]->GetActorLocation().FVector::Equals(ResetLocations[i], 5.f))ResetPuzzle();
			}
		}
		
	}
}

void UStatuePuzzleLock::CreateNewPermutation()
{
	StatuesToGuess.Empty();
	for (int i = 0; i < LightsAndTriggers.Num(); ++i)
	{
		StatuesToGuess.Add(AllStatues[FMath::RandRange(0, AllStatues.Num() - 1)]);
		UE_LOG(LogTemp,Warning,TEXT("statue with material %s"), *StatuesToGuess[i]->FindComponentByClass<UPrimitiveComponent>()->GetMaterial(0)->GetName())
	}
}

void UStatuePuzzleLock::LightColourEvaluation()
{
	for (int i = 0; i < LightsAndTriggers.Num(); ++i)
	{
		TArray<UPrimitiveComponent*> statueInTrigger;
		if (!LightsAndTriggers[i].theTrigger)break;
		LightsAndTriggers[i].theTrigger->GetOverlappingComponents(statueInTrigger);
		if (statueInTrigger.Num() > 0)
		{
			UMaterialInterface* theMaterial = statueInTrigger[0]->GetMaterial(0);
			if (theMaterial == StatuesToGuess[i]->FindComponentByClass<UPrimitiveComponent>()->GetMaterial(0))
			{
				SetLightGreen(LightsAndTriggers[i].theLight);
				continue;
			}
			for (int j = 0; j < LightsAndTriggers.Num(); j++)
			{
				if (theMaterial == StatuesToGuess[j]->FindComponentByClass<UPrimitiveComponent>()->GetMaterial(0))
				{
					SetLightYellow(LightsAndTriggers[i].theLight);
					break;
				}
			}
		}
		else
		{
			SetLightRed(LightsAndTriggers[i].theLight);
		}
	}
}

void UStatuePuzzleLock::ResetPuzzle()
{
	for (int i = 0; i < AllStatues.Num();++i)
	{
		AllStatues[i]->SetActorLocation(ResetLocations[i]);
		AllStatues[i]->SetActorRotation(ResetRotations[i]);
		
	}
	CreateNewPermutation();
}

void UStatuePuzzleLock::SetLightGreen(ASpotLight * light)
{
	light->SetLightColor(FLinearColor(0, 1, 0));
	for (int i = 0; i < LightsAndTriggers.Num(); ++i)
	{
		if (LightsAndTriggers[i].theLight->GetLightColor() != FLinearColor(0, 1, 0))return;
	}
	OpenRequest.Broadcast();
}
void UStatuePuzzleLock::SetLightYellow(ASpotLight * light)
{
	light->SetLightColor(FLinearColor(1, 1, 0));
}
void UStatuePuzzleLock::SetLightRed(ASpotLight * light)
{
	light->SetLightColor(FLinearColor(1, 0, 0));
	CloseRequest.Broadcast();
}

