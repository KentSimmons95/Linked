// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LinkedPlayerController.generated.h"

class ALinkedGameMode;

enum EFaceDirection;
enum EMoveDirection;
enum ELevels;

/**
 * 
 */
UCLASS()
class LINKED_API ALinkedPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	//Allows the PlayerPawns to register themselves to the Controller based on their Actor Tag
	void RegisterPlayerPawns(class ALinkedPlayerPawn* PlayerPawn);

	//Returns how many moves the player controller can make in the level
	UFUNCTION(BlueprintCallable)
	int32 GetNumberOfMovesRemaining() const;

	//Sets how many moves the player controller can make in the level
	void SetNumberOfMovesRemaining(ELevels CurrentLevel);

private:

	UPROPERTY(VisibleAnywhere, Category = "PlayerPawns")
	ALinkedPlayerPawn* LeftPawn;
	UPROPERTY(VisibleAnywhere, Category = "PlayerPawns")
	ALinkedPlayerPawn* RightPawn;
	
	ALinkedGameMode* GameMode = nullptr;
	
	bool LeftPawnRegistered = false;
	bool RightPawnRegistered = false;

	//The number of moves that the pawn is allowed to make in the level
	int32 NumberOfMovesRemaining = 0;

	bool GetLinkedGameMode();

	//Enable Input and setup Action Bindings for each Pawn
	void SetupInput();

	void LeftPawnMoveUp();
	void LeftPawnMoveDown();
	void LeftPawnMoveLeft();
	void LeftPawnMoveRight();

	void RightPawnMoveUp();
	void RightPawnMoveDown();
	void RightPawnMoveLeft();
	void RightPawnMoveRight();

	void SubtractOneFromMovesRemaining();
	//Checks if there is 1 or more remaining moves avaible
	bool HaveEnoughPointsToMove();

	//Default set to LeftPawn Push
	void PawnPush();
	//Default set to RightPawn Pull
	void PawnPull();

	//Check if the Left and Right pawn are facing the same direction
	bool IsFacingSameDirection(EFaceDirection FaceDirection);
	//Make the Left and Right pawn face the same direction
	void TurnSameDirection(EFaceDirection FaceDirection);
	//Check if the Left and Right pawn can both move in the same direction
	bool CanBothMoveInDirection(EMoveDirection MoveDirection);
	//Move Left and Right pawn in the specified direction - only use once you know they can both move in the specified direction
	void MoveBothInDirection(EMoveDirection MoveDirection);

};
