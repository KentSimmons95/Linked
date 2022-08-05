// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Actors/Tile.h"
#include "LinkedPlayerPawn.generated.h"

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

UCLASS()
class LINKED_API ALinkedPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALinkedPlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveUp(float AxisValue);
	void MoveRight(float AxisValue);

	//Gets the current tile the pawn is standing on
	ATile* GetCurrentTile() const;

	//Checks each direction that the pawn can move in
	bool CanMoveUp(FTileNeighbours& Neighbours);
	bool CanMoveDown(FTileNeighbours& Neighbours);
	bool CanMoveLeft(FTileNeighbours& Neighbours);
	bool CanMoveRight(FTileNeighbours& Neighbours);

private:
	UGameplayStatics* GameplayStatics;
	class ALinkedPlayerController* PlayerController;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, Category = "Pawn Information")
	ATile* PawnStartTile;
	UPROPERTY(VisibleAnywhere, Category = "Pawn Information")
	ATile* CurrentTile;

	UPROPERTY(VisibleAnywhere, Category = "Movable Tiles")
	FCurrentPossibleMoves PossibleMoves;

	FVector MovementDirection;
	UPROPERTY()
	float MovementSpeed = 500.f;

	void SetupStartingPosition();
	void GetPossibleMoves();

};
