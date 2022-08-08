// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Actors/Tile.h"
#include "Components/TileMovementComponent.h"

#include "GameplayTagContainer.h"

#include "LinkedPlayerPawn.generated.h"

class ALinkedPlayerController;

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

	//Gets the current tile the pawn is standing on
	ATile* GetCurrentTile() const;
	void SetCurrentTile(ATile* NewTile);

	void UpdateTileNeighbours();
	ATile* GetNeighbouringTileUp() const;
	ATile* GetNeighbouringTileDown() const;
	ATile* GetNeighbouringTileLeft() const;
	ATile* GetNeighbouringTileRight() const;
	FTileNeighbours GetCurrentTileNeighbours() const;
	

	//Set of functions that can be called to move the Pawn on the tiles
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();


private:
	UGameplayStatics* GameplayStatics;
	ALinkedPlayerController* PlayerController;

	//Components
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(EditAnywhere)
	UTileMovementComponent* TileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Pawn Information")
	ATile* PawnStartTile;
	UPROPERTY(VisibleAnywhere, Category = "Pawn Information")
	ATile* CurrentTile;

	FTileNeighbours CurrentTileNeighbours;

	FVector MovementDirection;
	UPROPERTY()
	float MovementSpeed = 500.f;

	void SetupStartingPosition();
};
