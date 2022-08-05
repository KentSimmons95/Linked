// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

//Struct to store the tiles coordinate in the tileset
USTRUCT()
struct FTileCoordinate
{
	GENERATED_BODY()

	uint32 X;
	uint32 Y;
};

//Struct to manage and keep track of a tiles neighbours
USTRUCT()
struct FTileNeighbours
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, Category = "Neighbours")
	ATile* UpNeighbour = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Neighbours")
	ATile* DownNeighbour = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Neighbours")
	ATile* LeftNeighbour = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Neighbours")
	ATile* RightNeighbour = nullptr;
};

UCLASS()
class LINKED_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	//TileManager is a friend to Tile
	friend class ATileManager;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//XValue translates to the Width position in the tileset
	//YValue translates to the Height position in the tileset
	void SetTileCoords(uint32 XValue, int32 YValue);

	FTileNeighbours GetTileNeighbours() const;
		 
	uint32 GetXCoord();
	uint32 GetYCoord();

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComp;

	//X and Y coordinate of the tile in the tileset
	FTileCoordinate TileCoord;
	UPROPERTY(VisibleAnywhere, Category = "Neighbours")
	FTileNeighbours TileNeighbours;

	void AddUpNeighbour(ATile* UpNeighbour);
	void AddDownNeighbour(ATile* DownNeighbour);
	void AddLeftNeighbour(ATile* LeftNeighbour);
	void AddRightNeighbour(ATile* RightNeighbour);
};


