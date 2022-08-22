// Fill out your copyright notice in the Description page of Project Settings.


#include "World/LinkedGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/RuneExit.h"

void ALinkedGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("LinkedGameMode --- BeginPlay called ---"));

	//Get the name of the level as an ELevel enum
	DetermineLevelAsEnum();
}

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
			UE_LOG(LogTemp, Warning, TEXT("The level has ended!"));

			//Transition animation

			//Load next level
			LoadNextLevel();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find registered RuneExitOne OR RuneExitTwo"));
	}
}

ELevels ALinkedGameMode::GetCurrentLevel() const
{
	return ECurrentLevel;
}

void ALinkedGameMode::DetermineLevelAsEnum()
{
	//UGameplayStatics* GameStatics;

	CurrentLevelName = GameStatics->GetCurrentLevelName(GetWorld());
	UE_LOG(LogTemp, Warning, TEXT("CurrentLevelName: %s"), *CurrentLevelName);

	if (CurrentLevelName == MainMenu)
	{
		ECurrentLevel = ELevels::Level0;
	}
	else if (CurrentLevelName == Level1)
	{
		ECurrentLevel = ELevels::Level1;
	}
	else if(CurrentLevelName == Level2)
	{
		ECurrentLevel = ELevels::Level2;
	}
	else if (CurrentLevelName == Level3)
	{
		ECurrentLevel = ELevels::Level3;
	}
	else if (CurrentLevelName == Level4)
	{
		ECurrentLevel = ELevels::Level4;
	}
	else if (CurrentLevelName == Level5)
	{
		ECurrentLevel = ELevels::Level5;
	}
	else if (CurrentLevelName == Level6)
	{
		ECurrentLevel = ELevels::Level6;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to determine the CurrentLevelName!"));
	}
}

void ALinkedGameMode::LoadNextLevel()
{
	//UGameplayStatics* GameStatics;

	switch (ECurrentLevel)
	{
		//Level0 is the main menu
	case ELevels::Level0:
		//LoadLevel(1)
		GameStatics->OpenLevel(GetWorld(), FName("Level1"));
		break;
	case ELevels::Level1:
		//LoadLevel(2)
		GameStatics->OpenLevel(GetWorld(), FName("Level2"));
		break;
	case ELevels::Level2:
		//LoadLevel(3)
		GameStatics->OpenLevel(GetWorld(), FName("Level3"));
		break;
	case ELevels::Level3:
		//LoadLevel(4)
		GameStatics->OpenLevel(GetWorld(), FName("Level4"));
		break;
	case ELevels::Level4:
		//LoadLevel(5)
		GameStatics->OpenLevel(GetWorld(), FName("Level5"));
		break;
	case ELevels::Level5:
		//LoadLevel(6)
		GameStatics->OpenLevel(GetWorld(), FName("Level6"));
		break;
	case ELevels::Level6:
		//LoadLevel(0) - MainMenu
		GameStatics->OpenLevel(GetWorld(), FName("MainMenuLevel"));
		break;
	default:
		//LoadLevel(0) - MainMenu
		UE_LOG(LogTemp, Warning, TEXT("Failed to find level to load - loading main menu as default!"));
		GameStatics->OpenLevel(GetWorld(), FName("MainMenuLevel"));
		break;
	}
}
