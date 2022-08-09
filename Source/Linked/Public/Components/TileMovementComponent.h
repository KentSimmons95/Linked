// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actors/Tile.h"
#include "Components/TimelineComponent.h"
#include "TileMovementComponent.generated.h"

class UCurveFloat;
class ALinkedPlayerPawn;

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

UENUM()
enum EMoveDirection
{
	None	 UMETA(DisplayName = "None"),
	Up		 UMETA(DisplayName = "Up"),
	Down	 UMETA(DisplayName = "Down"),
	Left	 UMETA(DisplayName = "Left"),
	Right	 UMETA(DisplayName = "Right")
};

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

	//TODO - Move or Remove?
	ATile* GetNeighbouringTileUp() const;
	ATile* GetNeighbouringTileDown() const;
	ATile* GetNeighbouringTileLeft() const;
	ATile* GetNeighbouringTileRight() const;
	FTileNeighbours GetCurrentTileNeighbours() const;
	
	//Moves to pawn to the chosen tile
	void MoveToTile(EMoveDirection Direction);

private:

	/**** TIMELINE ****/
	FTimeline CurveTimeline;
	UPROPERTY(EditAnywhere)
	UCurveFloat* CurveFloat = nullptr;
	FVector StartLoc;
	FVector EndLoc;

	/*** TILE NEIGHBOUR TRACKING **/
	UPROPERTY(EditAnywhere, Category = "Pawn Information")
	ATile* PawnStartTile = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Tiles")
	ATile* CurrentTile = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Tiles")
	ATile* TileMovedTo = nullptr;

	/*** MOVEMENT **/
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

	FTileNeighbours CurrentTileNeighbours;

	UPROPERTY(VisibleAnywhere, Category = "Pawn Information")
	bool IsMoving = false;
	
	//The ALinkedPlayerPawn that this component is attached to
	AActor* PawnOwner = nullptr;

	UPROPERTY(EditAnywhere, Category = "Interp Speed")
	float InterpSpeed = 250;

	UPROPERTY(VisibleAnywhere, Category = "Movable Tiles")
	FCurrentPossibleMoves PossibleMoves;

	void GetPossibleMoves();
	//Checks each direction that the pawn can move in
	bool CanMoveUp() const;
	bool CanMoveDown() const;
	bool CanMoveLeft() const;
	bool CanMoveRight() const;

	/*Function to check that all the required fields are initiated in editor
	 *Called at BeginPlay() 
	 */
	void CheckVariables();
	void InitTimeline();


	//Controls how the Actor lerps between StartLoc and EndLoc 
	UFUNCTION()
	void TimelineProgress(float Value);

	//Calls UpdatePawnInformation() after Timeline is Complete
	FOnTimelineEvent OnMoveCompleted();
	//Called by OnMoveCompleted() by FOnTimelineEvent - used to update the owning Pawns information
	UFUNCTION()
	void UpdatePawnInformation();
};
