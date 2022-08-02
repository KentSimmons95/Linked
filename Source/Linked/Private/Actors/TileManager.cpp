// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TileManager.h"
#include "Actors/Tile.h"

// Sets default values
ATileManager::ATileManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATileManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATileManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	//If there is an existing tileset spawned - clear clear them
	for (auto Tile : Tiles)
	{
		Tile->Destroy();
	}
	Tiles.Empty();
	

	//Used to pass into GetTileSpawnLocation()
	FVector TileManagerLocation = GetActorLocation();

	//Spawn the set of our tile actors
	for (int h = 0; h < RoomHeight; ++h)
	{
		for (int w = 0; w < RoomWidth; ++w)
		{
			FVector ActorToSpawnLocation;
			FRotator ActorToSpawnRotation;
			GetTileSpawnLocation(h, w, TileManagerLocation, ActorToSpawnLocation);
			ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileActor, ActorToSpawnLocation, ActorToSpawnRotation);

			//Attach the newly spawned Actors to the Plot Actor and keep its World Transform
			NewTile->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			Tiles.Add(NewTile);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Generated Tile Success"));
}

