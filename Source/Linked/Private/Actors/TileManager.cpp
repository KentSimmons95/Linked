// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TileManager.h"
#include "Actors/Tile.h"

// Sets default values
ATileManager::ATileManager()
{

}

// Called when the game starts or when spawned
void ATileManager::BeginPlay()
{
	Super::BeginPlay();
	
}


void ATileManager::GetTileSpawnLocation(int32 Height, int32 Width, FVector& ManagerLocation, FVector& OutSpawnLocation)
{
	OutSpawnLocation.X = ManagerLocation.X + (Height * 100);
	OutSpawnLocation.Y = ManagerLocation.Y + (Width * 100);
}

void ATileManager::GenerateTileSet()
{
	if (!TileActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Empty or invalid TileActor in: TileManager!"));
		return;
	}

	ResetTiles();
	SpawnTiles();

	//After each tile has been spawned - determine their neighbours
	//A tile is considered a neighbough if it is: Directly Above/Below/Left/Right of the current tile
	DetermineTileNeighbours();

	UE_LOG(LogTemp, Warning, TEXT("Generated Tile Success"));
}

void ATileManager::ResetTiles()
{
	for (auto Tile : Tiles)
	{
		Tile->Destroy();
	}
	Tiles.Empty();
}

void ATileManager::SpawnTiles()
{
	//Used to pass into GetTileSpawnLocation()
	FVector TileManagerLocation = GetActorLocation();

	//Spawn the set of tile actors
	for (int h = 0; h < RoomHeight; ++h)
	{
		for (int w = 0; w < RoomWidth; ++w)
		{
			FVector ActorToSpawnLocation;
			FRotator ActorToSpawnRotation;
			GetTileSpawnLocation(h, w, TileManagerLocation, ActorToSpawnLocation);
			ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileActor, ActorToSpawnLocation, ActorToSpawnRotation);
			
			//Assign the new tile its current X and Y Index
			NewTile->SetTileCoords(w, h);

			//Attach the newly spawned Actors to the Plot Actor and keep its World Transform
			NewTile->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			Tiles.Add(NewTile);
		}
	}
	//After all the tiles are spawned - get each tiles neighbours
	DetermineTileNeighbours();
}

void ATileManager::DetermineTileNeighbours()
{
	if (Tiles.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Tiles array is Empty!"));
		return;
	}

	int32 CurrentArrayIndex = 0;

	//Iterate through each tile in the tiles array
	//Using their own tile coordinate determine which neighbours they have
	for (ATile* Tile : Tiles)
	{
		uint32 TileXCoord = 0;
		uint32 TileYCoord = 0;

		TileXCoord = Tile->GetXCoord();
		TileYCoord = Tile->GetYCoord();
		
		GetUpNeighbour(   Tile, TileYCoord, CurrentArrayIndex);
		GetDownNeighbour( Tile, TileYCoord, CurrentArrayIndex);
		GetRightNeighbour(Tile, TileXCoord, CurrentArrayIndex);
		GetLeftNeighbour( Tile, TileXCoord, CurrentArrayIndex);

		++CurrentArrayIndex;
	}
}

void ATileManager::GetUpNeighbour(ATile* CurrentTile, uint32& YCoord, int32 ArrayIndex)
{
	//If the Y-Coordinate is (RoomHeight - 1) there are no neighbours above (top most row of tiles)
	if (YCoord == (RoomHeight - 1))
	{
		return;
	}
	else
	{
		//Right neighbour is the tile at the current array index + RoomHeight
		CurrentTile->AddUpNeighbour(Tiles[ArrayIndex + RoomHeight]);
	}
}

void ATileManager::GetDownNeighbour(ATile* CurrentTile, uint32& YCoord, int32 ArrayIndex)
{
	//If the Y-Coordinate is 0 - there are no neighbours below (bottom row of tiles)
	if (YCoord == 0)
	{
		return;
	}
	else
	{
		//Down neighbour is the tile at the current array index - RoomWidth
		CurrentTile->AddDownNeighbour(Tiles[ArrayIndex - RoomWidth]);
	}
}

void ATileManager::GetLeftNeighbour(ATile* CurrentTile, uint32& XCoord, int32 ArrayIndex)
{
	//If the X-Coordinate is 0 - there are no neighbours to the left (left most row of tiles)
	if (XCoord == 0)
	{
		return;
	}
	else
	{
		//Left neighbour is the tile at the current array index -1
		CurrentTile->AddLeftNeighbour(Tiles[ArrayIndex - 1]);
	}
}

void ATileManager::GetRightNeighbour(ATile* CurrentTile, uint32& XCoord, int32 ArrayIndex)
{
	//If the X-Coordinate is (RoomWidth - 1) there are no neighbours to the right (right most row of tiles)
	if (XCoord == (RoomWidth - 1))
	{
		return;
	}
	else
	{
		//Right neighbour is the tile at the current array index + 1
		CurrentTile->AddRightNeighbour(Tiles[ArrayIndex + 1]);
	}
}

