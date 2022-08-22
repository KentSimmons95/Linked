// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Block.h"

// Sets default values
ABlock::ABlock()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	TileMovementComponent = CreateDefaultSubobject<UTileMovementComponent>(TEXT("TileMovementComponent"));
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();

	if (!TileMovementComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find TileMovementComponent on Actor - %s!"), *this->GetActorNameOrLabel());
	}

	//Old Assert
	//checkf(TileMovementComponent, TEXT("Failed to find TileMovementComponent on Actor - %s!"), *this->GetActorNameOrLabel());
}

bool ABlock::CanMoveInDirection(EMoveDirection Direction) const
{
	return TileMovementComponent->CanMoveInDirection(Direction);
}

void ABlock::MoveInDirection(EMoveDirection Direction) const
{
	TileMovementComponent->MoveToTile(Direction);
}

void ABlock::MoveToTileLocation()
{
	ATile* StartTileLocation = TileMovementComponent->GetActorStartTile();

	if (!StartTileLocation)
	{
		UE_LOG(LogTemp, Warning, TEXT("TileMovementComponent StartTile is empty for Block Actor: %s"), *this->GetActorNameOrLabel());
	}
	else
	{
		this->SetActorLocation(StartTileLocation->GetActorLocation());
	}
}


