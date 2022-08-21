// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Tile.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->AttachToComponent(SceneRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	//RootComponent = StaticMeshComp;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::SetTileCoords(uint32 XValue, int32 YValue)
{
	TileCoord.X = XValue;
	TileCoord.Y = YValue;
}

bool ATile::HasActorOnTile() const
{
	return bHasActorOnTile;
}

void ATile::UpdateActorOnTile(AActor* Actor)
{
	if (Actor == nullptr)
	{
		ActorOnTile = nullptr;
		bHasActorOnTile = false;
	}
	else
	{
		ActorOnTile = Actor;
		bHasActorOnTile = true;
	}
}

AActor* ATile::GetActorOnTile() const
{
	return ActorOnTile;
}

void ATile::AddUpNeighbour(ATile* UpNeighbour)
{
	TileNeighbours.UpNeighbour = UpNeighbour;
}

void ATile::AddDownNeighbour(ATile* DownNeighbour)
{
	TileNeighbours.DownNeighbour = DownNeighbour;
}

void ATile::AddLeftNeighbour(ATile* LeftNeighbour)
{
	TileNeighbours.LeftNeighbour = LeftNeighbour;

}

void ATile::AddRightNeighbour(ATile* RightNeighbour)
{
	TileNeighbours.RightNeighbour = RightNeighbour;

}

FTileNeighbours ATile::GetTileNeighbours() const
{
	return TileNeighbours;
}

uint32 ATile::GetXCoord()
{
	return TileCoord.X;
}

uint32 ATile::GetYCoord()
{
	return TileCoord.Y;
}



