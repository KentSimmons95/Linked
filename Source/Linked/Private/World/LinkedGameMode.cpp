// Fill out your copyright notice in the Description page of Project Settings.


#include "World/LinkedGameMode.h"
#include "Actors/RuneExit.h"

bool ALinkedGameMode::RegisterRuneExit(ARuneExit* RuneExit)
{
	bool HasRegistered = false;
	//Check tag on each the RuneExit actor and assign it to the appropriate RuneExit(X)
	if (RuneExit->ActorHasTag(FName("RuneExitOne")))
	{
		RuneExitOne = RuneExit;
		HasRegistered = true;
		return HasRegistered;
	}

	if (RuneExit->ActorHasTag(FName("RuneExitTwo")))
	{
		RuneExitTwo = RuneExit;
		HasRegistered = true;
		return HasRegistered;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Failed to register a RuneExit!"));
	return HasRegistered;
}

void ALinkedGameMode::CheckIfEndOfLevel()
{
	if (RuneExitOne && RuneExitTwo)
	{
		if (RuneExitOne->IsPawnStandingOnExit() && RuneExitTwo->IsPawnStandingOnExit())
		{
			//Trigger end of level events (Disable input and make actors float upwards/beam me up Scotty)
			UE_LOG(LogTemp, Warning, TEXT("The level has ended!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find registered RuneExitOne OR RuneExitTwo"));
	}
}
