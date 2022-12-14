// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DirectionComponent.generated.h"

class ALinkedPlayerPawn;

//Enum used to rotate the actor in the desired direction
UENUM()
enum EFaceDirection
{
	FaceUp		 UMETA(DisplayName = "Up"),
	FaceDown	 UMETA(DisplayName = "Down"),
	FaceLeft	 UMETA(DisplayName = "Left"),
	FaceRight	 UMETA(DisplayName = "Right")
};

/* Direction Component  
   Attaches to a Pawn to help determine which 
   direction they are facing when calculating movement on the tiles		
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LINKED_API UDirectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDirectionComponent();

protected:

	virtual void BeginPlay() override;

public:	

	//Rotates the actor in the given EFaceDirection
	void RotateActor(EFaceDirection Direction);

	//Returns the CurrentFacingDirection
	EFaceDirection GetCurrentFaceDirection() const;

private:

	//This components PawnOwner
	ALinkedPlayerPawn* PawnOwner = nullptr;

	//Set the default direction to be FaceUp
	EFaceDirection CurrentFacingDirection = EFaceDirection::FaceUp;
	FRotator TurnUp    = { 0.0f, 0.0f   , 0.0f};
	FRotator TurnDown  = { 0.0f, 180.0f , 0.0f};
	FRotator TurnLeft  = { 0.0f, 270.0f , 0.0f};
	FRotator TurnRight = { 0.0f, 90.0f  , 0.0f};

	// Rotation fucntions - Called by RotateActor to TURN the parent actor in the desired direction
	void FaceUp();
	void FaceDown();
	void FaceLeft();
	void FaceRight();
	
};
