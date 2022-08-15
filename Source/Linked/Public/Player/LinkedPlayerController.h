// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LinkedPlayerController.generated.h"

//Forward declare the enums from DirectionComponent.h and TileMovementComponent.h

enum EFaceDirection;
enum EMoveDirection;

/**
 * 
 */
UCLASS()
class LINKED_API ALinkedPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	//Allows the PlayerPawns to register themselves to the Controller based on their Tag
	void RegisterPlayerPawns(class ALinkedPlayerPawn* PlayerPawn);

private:

	UPROPERTY(VisibleAnywhere, Category = "PlayerPawns")
	ALinkedPlayerPawn* LeftPawn;
	UPROPERTY(VisibleAnywhere, Category = "PlayerPawns")
	ALinkedPlayerPawn* RightPawn;
	
	//TODO - Remove?
	bool CanPawnsSeeEachOther = true;

	bool LeftPawnRegistered = false;
	bool RightPawnRegistered = false;

	void SetupInput();

	void LeftPawnMoveUp();
	void LeftPawnMoveDown();
	void LeftPawnMoveLeft();
	void LeftPawnMoveRight();

	void RightPawnMoveUp();
	void RightPawnMoveDown();
	void RightPawnMoveLeft();
	void RightPawnMoveRight();

	void LeftPawnPush();
	void RightPawnPull();

	//Check if the Left and Right pawn are facing the same direction
	bool IsFacingSameDirection(EFaceDirection FaceDirection);
	//Make the Left and Right pawn face the same direction
	void TurnSameDirection(EFaceDirection FaceDirection);
	//Check if the Left and Right pawn can both move in the same direction
	bool CanBothMoveInDirection(EMoveDirection MoveDirection);
	//Move Left and Right pawn in the specified direction - only use once you know they can both move in the specified direction
	void MoveBothInDirection(EMoveDirection MoveDirection);
	

	void Interact();
};
