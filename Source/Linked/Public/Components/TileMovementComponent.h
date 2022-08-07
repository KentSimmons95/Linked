// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actors/Tile.h"
#include "TileMovementComponent.generated.h"


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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	//Checks each direction that the pawn can move in
	bool CanMoveUp() const;
	bool CanMoveDown() const;
	bool CanMoveLeft() const;
	bool CanMoveRight() const;

	//Moves to pawn to the chosen tile
	void MoveToTile(float DeltaTime, ATile* Tile);

	UFUNCTION(CallInEditor, Category = "Movement")
	void MoveUp(float DeltaTime);
	UFUNCTION(CallInEditor, Category = "Movement")
	void MoveDown(float DeltaTime);
	UFUNCTION(CallInEditor, Category = "Movement")
	void MoveLeft(float DeltaTime);
	UFUNCTION(CallInEditor, Category = "Movement")
	void MoveRight(float DeltaTime);

private:

	//The ALinkedPlayerPawn that this component is attached to
	class ALinkedPlayerPawn* PawnOwner = nullptr;

	UPROPERTY(EditAnywhere, Category = "Interp Speed")
	float InterpSpeed = 150;

	UPROPERTY(VisibleAnywhere, Category = "Movable Tiles")
	FCurrentPossibleMoves PossibleMoves;

	void GetPossibleMoves();
};
