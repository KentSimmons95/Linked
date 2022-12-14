// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actors/Tile.h"
#include "Components/TimelineComponent.h"
#include "TileMovementComponent.generated.h"

class UCurveFloat;

USTRUCT()
struct FCurrentPossibleMoves
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Movable Tiles")
	bool bCanMoveUp = false;
	UPROPERTY(VisibleAnywhere, Category = "Movable Tiles")
	bool bCanMoveDown = false;
	UPROPERTY(VisibleAnywhere, Category = "Movable Tiles")
	bool bCanMoveLeft = false;
	UPROPERTY(VisibleAnywhere, Category = "Movable Tiles")
	bool bCanMoveRight = false;
};

//Enum to move the actor in the desired direction
UENUM()
enum EMoveDirection
{
	Up		 UMETA(DisplayName = "Up"),
	Down	 UMETA(DisplayName = "Down"),
	Left	 UMETA(DisplayName = "Left"),
	Right	 UMETA(DisplayName = "Right"),
	None	 UMETA(DisplayName = "None")
};

// Actor Component that allows the owning actor to move on the Tile System
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LINKED_API UTileMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTileMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	ATile* GetCurrentTile() const;
	void SetCurrentTile(ATile* NewTile);
	void UpdateTileNeighbours();

	//TODO - Remove?
	FTileNeighbours GetCurrentTileNeighbours() const;
	
	//Check if we have completed the move
	bool HasMoveCompleted();

	//Returns if the owning actor is currently moving or not
	bool IsCurrentlyMoving();
	
	//Check if the direction to move in is possible
	bool CanMoveInDirection(EMoveDirection Direction);

	//Moves the actor to the chosen tile
	void MoveToTile(EMoveDirection Direction);

	//Returns the starting tile that is assigned to the actor
	ATile* GetActorStartTile() const;
private:

	/**** Timeline properties ****/
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* CurveFloat = nullptr;
	FTimeline CurveTimeline;
	FVector StartLoc;
	FVector EndLoc;

	/**** Tile neighbour tracking ****/
	UPROPERTY(EditAnywhere, Category = "Actor Information")
	ATile* ActorStartTile = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Tiles")
	ATile* CurrentTile = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Tiles")
	ATile* TileMovedTo = nullptr;

	/**** Movement - Called by MoveToTile to MOVE the parent actor in the desired direction ****/
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

	//Movement direction checking
	bool CanMoveUp()    const;
	bool CanMoveDown()  const;
	bool CanMoveLeft()  const;
	bool CanMoveRight() const;

	FTileNeighbours CurrentTileNeighbours;

	UPROPERTY(VisibleAnywhere, Category = "Actor Information")
	bool IsMoving = false;
	
	//The AActor that this component is attached to
	AActor* ActorOwner = nullptr;

	UPROPERTY(EditAnywhere, Category = "Interp Speed")
	float InterpSpeed = 250;

	UPROPERTY(VisibleAnywhere, Category = "Movable Tiles")
	FCurrentPossibleMoves PossibleMoves;

	void GetPossibleMoves();

	/*Function to check that all the required fields are initiated in editor
	 *Called at BeginPlay() 
	 */
	void CheckVariables();
	void InitTimeline();

	//Controls how the Actor lerps between StartLoc and EndLoc 
	UFUNCTION()
	void TimelineProgress(float Value);

	//Calls UpdateActorInformation() after Timeline is Complete
	FOnTimelineEvent OnMoveCompleted();
	//Called by OnMoveCompleted() by FOnTimelineEvent - used to update the owning Actors information
	UFUNCTION()
	void UpdateActorInformation();
};
