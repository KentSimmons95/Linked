// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LinkedPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LINKED_API ALinkedPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	//Allows the PlayerPawns to register themselves to the Controller
	bool RegisterPlayerPawns(class ALinkedPlayerPawn* PlayerPawn);

private:

	UPROPERTY(VisibleAnywhere, Category = "PlayerPawns")
	TArray<class ALinkedPlayerPawn*> Pawns;
	int32 NumPawns = 0;

	bool CanPawnsSeeEachOther = true;

	void SetupInput();


	//Trace a line of sight between the 2 Pawns to see if they are connected
	//If the trace is blocked by an object then disable certain movement
	bool HasLineOfSight();

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
};
