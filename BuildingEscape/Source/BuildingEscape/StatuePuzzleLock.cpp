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
	CreateNewPermutation();
	
}

// Called every frame
void UStatuePuzzleLock::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	if (ResetTrigger)
	{
		if (ResetTrigger->IsOverlappingActor(GetWorld()->GetFirstPlayerController()->GetPawn()))
		{
			///We want to reset the puzzle if the person solves it and escapes
			ResetPuzzle();
		}
		else LightColourEvaluation();
	}
}

void UStatuePuzzleLock::CreateNewPermutation()
{
	StatuesToGuess.SetNumZeroed(LightsAndTriggers.Num());
	for (int i = 0; i < LightsAndTriggers.Num(); ++i)
	{
		StatuesToGuess[i] = AllStatues[FMath::RandRange(0, AllStatues.Num() - 1)];
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
	for (int i = 0; i < LightsAndTriggers.Num(); ++i)
	{
		TArray<AActor*> statueInTrigger;
		if (!LightsAndTriggers[i].theTrigger || !ResetLocations.IsValidIndex(i))return;
		LightsAndTriggers[i].theTrigger->GetOverlappingActors(statueInTrigger);
		for (int32 j = 0; j < statueInTrigger.Num(); ++j)
		{
			statueInTrigger[j]->SetActorLocation(ResetLocations[i]);
			if (i == (LightsAndTriggers.Num()-1))CreateNewPermutation();
		}
		/*else
		{
			UE_LOG(LogTemp, Warning, TEXT("Not enough reset locations"))
			return;
		}*/
	}
	
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

