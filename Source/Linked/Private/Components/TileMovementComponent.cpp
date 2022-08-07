// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TileMovementComponent.h"
#include "Player/LinkedPlayerPawn.h"

// Sets default values for this component's properties
UTileMovementComponent::UTileMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTileMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	//Get the pawn this component is attached to
	PawnOwner = Cast<ALinkedPlayerPawn>(this->GetOwner());
	GetPossibleMoves();
}


// Called every frame
void UTileMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UTileMovementComponent::CanMoveUp() const
{
	//Check up tile
	if (PawnOwner->GetCurrentTileNeighbours().UpNeighbour == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool UTileMovementComponent::CanMoveDown() const
{
	//Check down tile
	if (PawnOwner->GetCurrentTileNeighbours().DownNeighbour == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool UTileMovementComponent::CanMoveLeft() const
{
	//Check left tile
	if (PawnOwner->GetCurrentTileNeighbours().LeftNeighbour == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool UTileMovementComponent::CanMoveRight() const
{
	//Check right tile
	if (PawnOwner->GetCurrentTileNeighbours().RightNeighbour == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void UTileMovementComponent::MoveToTile(float DeltaTime, ATile* Tile)
{
	PawnOwner->SetActorLocation(FMath::VInterpConstantTo(
		PawnOwner->GetActorLocation(),
		Tile->GetActorLocation(),
		DeltaTime,
		InterpSpeed));
}

void UTileMovementComponent::MoveUp(float DeltaTime)
{
	if (CanMoveUp())
	{
		MoveToTile(DeltaTime, PawnOwner->GetCurrentTileNeighbours().UpNeighbour);
	}
}

void UTileMovementComponent::MoveDown(float DeltaTime)
{
	if (CanMoveDown())
	{
		MoveToTile(DeltaTime, PawnOwner->GetCurrentTileNeighbours().DownNeighbour);
	}
}

void UTileMovementComponent::MoveLeft(float DeltaTime)
{
	if (CanMoveLeft())
	{
		MoveToTile(DeltaTime, PawnOwner->GetCurrentTileNeighbours().LeftNeighbour);
	}
}

void UTileMovementComponent::MoveRight(float DeltaTime)
{
	if (CanMoveRight())
	{
		MoveToTile(DeltaTime, PawnOwner->GetCurrentTileNeighbours().RightNeighbour);
	}
}

void UTileMovementComponent::GetPossibleMoves()
{
	// Check whether or not there is a neighour in each of the 4 possible directions 
	// If there is a tile - set flag to true or reset back to false if there isn't a tile

	PossibleMoves.bCanMoveUp = CanMoveUp();
	PossibleMoves.bCanMoveDown = CanMoveDown();
	PossibleMoves.bCanMoveLeft = CanMoveLeft();
	PossibleMoves.bCanMoveRight = CanMoveRight();

}

