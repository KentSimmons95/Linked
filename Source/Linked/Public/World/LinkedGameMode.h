// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LinkedGameMode.generated.h"

class ARuneExit;
class UGameplayStatics;
class ALinkedPlayerController;

UENUM()
enum ELevels
{
	Level0	UMETA(DisplayName = "Level0"),
	Level1	UMETA(DisplayName = "Level1"),
	Level2	UMETA(DisplayName = "Level2"),
	Level3	UMETA(DisplayName = "Level3"),
	Level4	UMETA(DisplayName = "Level4"),
	Level5	UMETA(DisplayName = "Level5"),
	Level6	UMETA(DisplayName = "Level6")
};

/** Handles game logic such as level switching and if the level is complete
 */
UCLASS()
class LINKED_API ALinkedGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	//Allows a Rune Exit to register themself if they have the correct Actor Tag
	bool RegisterRuneExit(ARuneExit* RuneExit);

	//Function that is called by the registered RuneExit actors to see if the level can be ended
	void CheckIfEndOfLevel();

private:

	UGameplayStatics* GameStatics;
	ALinkedPlayerController* PlayerController;

	//Keep track of the two exits in the level
	ARuneExit* RuneExitOne = nullptr;
	ARuneExit* RuneExitTwo = nullptr;

	//Stores the current level as an ELevels enum
	ELevels ECurrentLevel;
	//Stores the current level as a regular FString
	FString CurrentLevelName;

	//Determine the current level represented as an ELevels enum
	void DetermineLevelAsEnum();

	//Loads the next level based off the ECurrentLevel
	void LoadNextLevel();

	//Level names of the game (Improve to using data table?)
	const FString MainMenu = "MainMenuLevel";
	const FString Level1 = "Level1";
	const FString Level2 = "Level2";
	const FString Level3 = "Level3";
	const FString Level4 = "Level4";
	const FString Level5 = "Level5";
	const FString Level6 = "Level6";
};
