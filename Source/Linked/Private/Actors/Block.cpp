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

	checkf(TileMovementComponent, TEXT("Failed to find TileMovementComponent on Actor - %s!"), *this->GetActorNameOrLabel());
}

bool ABlock::CanMoveInDirection(EMoveDirection Direction)
{
	return TileMovementComponent->CanMoveInDirection(Direction);
}

void ABlock::MoveInDirection(EMoveDirection Direction)
{
	TileMovementComponent->MoveToTile(Direction);
}


