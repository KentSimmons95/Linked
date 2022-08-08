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

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LINKED_API UTileMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTileMovementComponent();

	/**** TIMELINE ****/
	UFUNCTION()
	void TimelineProgress(float Value);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/**** TIMELINE ****/
	FTimeline CurveTimeline;
	UPROPERTY(EditAnywhere)
	UCurveFloat* CurveFloat;

	FVector StartLoc;
	FVector EndLoc;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Moves to pawn to the chosen tile
	void MoveToTile(float DeltaTime, ATile* Tile);

	UFUNCTION(CallInEditor, Category = "Movement")
	void MoveUp();
	UFUNCTION(CallInEditor, Category = "Movement")
	void MoveDown(float DeltaTime);
	UFUNCTION(CallInEditor, Category = "Movement")
	void MoveLeft(float DeltaTime);
	UFUNCTION(CallInEditor, Category = "Movement")
	void MoveRight(float DeltaTime);

private:

	//The ALinkedPlayerPawn that this component is attached to
	ALinkedPlayerPawn* PawnOwner = nullptr;

	UPROPERTY(EditAnywhere, Category = "Interp Speed")
	float InterpSpeed = 150;

	UPROPERTY(VisibleAnywhere, Category = "Movable Tiles")
	FCurrentPossibleMoves PossibleMoves;

	//Variables to keep track of tiles
	UPROPERTY(VisibleAnywhere, Category = "Tiles")
	ATile* CurrentTile = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Tiles")
	ATile* TileMovedTo = nullptr;

	void GetPossibleMoves();

	//Checks each direction that the pawn can move in
	bool CanMoveUp() const;
	bool CanMoveDown() const;
	bool CanMoveLeft() const;
	bool CanMoveRight() const;

	//Calls UpdatePawnInformation() after Timeline is Complete
	FOnTimelineEvent OnMoveCompleted();
	UFUNCTION()
	void UpdatePawnInformation();
};
