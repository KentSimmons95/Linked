// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileManager.generated.h"

UCLASS()
class LINKED_API ATileManager : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATileManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	//The size of the room that the players can move around in
	UPROPERTY(EditAnywhere)
	int32 RoomHeight = 10;
	UPROPERTY(EditAnywhere)
	int32 RoomWidth = 10;

	UPROPERTY(EditAnywhere, Category = "Manager Details")
	TSubclassOf<class ATile> TileActor;

	//Get the TileManagers location
	FVector CurrentActorLocation;

	TArray<class ATile*> Tiles;

	void GetTileSpawnLocation(int32 Height, int32 Width, FVector& ManagerLocation, FVector& OutSpawnLocation);

	UFUNCTION(CallInEditor)
	void GenerateTileSet();
	void ResetTiles();
	void SpawnTiles();

	//Find each tiles neighbours
	void DetermineTileNeighbours();
	void GetUpNeighbour(ATile* CurrentTile, uint32& YCoord, int32 ArrayIndex);
	void GetDownNeighbour(ATile* CurrentTile, uint32& YCoord, int32 ArrayIndex);
	void GetLeftNeighbour(ATile* CurrentTile, uint32& XCoord, int32 ArrayIndex);
	void GetRightNeighbour(ATile* CurrentTile, uint32& XCoord, int32 ArrayIndex);
};
