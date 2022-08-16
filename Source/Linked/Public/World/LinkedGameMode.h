// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LinkedGameMode.generated.h"

class ARuneExit;

/**
 * 
 */
UCLASS()
class LINKED_API ALinkedGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	bool RegisterRuneExit(ARuneExit* RuneExit);

	//Function that is called by the registered RuneExit actors to see if the level can be ended
	void CheckIfEndOfLevel();

protected:

private:

	//Keep track of the two exits in the level
	ARuneExit* RuneExitOne = nullptr;
	ARuneExit* RuneExitTwo = nullptr;

};
