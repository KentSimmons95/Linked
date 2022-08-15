// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractComponent.h"
#include "Actors/Block.h"
#include "Actors/Tile.h"
#include "Player/LinkedPlayerPawn.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	PawnOwner = Cast<ALinkedPlayerPawn>(this->GetOwner());
}

void UInteractComponent::TryPushBlock()
{
	//Used to store the FaceDirection from GetTileInDirection()
	EFaceDirection FaceDirection;
	ATile* Tile = GetTileInDirection(FaceDirection);

	EMoveDirection BlockMoveDirection = DeterminePushDirection(FaceDirection);

	if (Tile)
	{
		UE_LOG(LogTemp, Warning, TEXT("Valid tile"));
		AActor* ActorOnTile = Tile->GetActorOnTile();

		//Early return if there is no actor on the tile
		if (!ActorOnTile)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Actor on tile!"));
			return;
		}

		if (ActorOnTile->IsA(ABlock::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("The actor on tile is a ABlock!"));
			ABlock* Block = Cast<ABlock>(ActorOnTile);

			//If block can move then move
			//Else do nothing
			if (Block->CanMoveInDirection(BlockMoveDirection))
			{
				UE_LOG(LogTemp, Warning, TEXT("Block can move in that direction!"));
				Block->MoveInDirection(BlockMoveDirection);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Block cannot move in that direction!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No block actor on tile"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid tile"));
	}
}

void UInteractComponent::TryPullBlock()
{
	UE_LOG(LogTemp, Warning, TEXT("TryPullBlock called"));

	EFaceDirection FaceDirection;
	ATile* SecondTile = GetSecondTileInDirection(FaceDirection);

	EMoveDirection BlockMoveDirection = DeterminePullDirection(FaceDirection);

	if (SecondTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("Valid second tile - name is: %s"), *SecondTile->GetActorNameOrLabel());
		AActor* ActorOnTile = SecondTile->GetActorOnTile();

		//Early return if there is no actor on the tile
		if (!ActorOnTile)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Actor on tile!"));
			return;
		}

		if (ActorOnTile->IsA(ABlock::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("The actor on tile is a ABlock!"));
			ABlock* Block = Cast<ABlock>(ActorOnTile);

			//If block can move then move
			//Else do nothing
			if (Block->CanMoveInDirection(BlockMoveDirection))
			{
				UE_LOG(LogTemp, Warning, TEXT("Block can move in that direction!"));
				Block->MoveInDirection(BlockMoveDirection);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Block cannot move in that direction!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No block actor on tile"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid second tile"));
	}

}

ATile* UInteractComponent::GetTileInDirection(EFaceDirection& OutDirection)
{
	//Get current tile we are on
	ATile* CurrentTile = PawnOwner->GetCurrentTile();
	ATile* FacingTile = nullptr;

	//Determine which direction the pawn is facing
	EFaceDirection CurrentFacingDirection = PawnOwner->GetCurrentFaceDirection();
	OutDirection = CurrentFacingDirection;

	//Calculate the neighbouring tile in the direction we are facing
	switch (CurrentFacingDirection)
	{
	case EFaceDirection::FaceUp:
		FacingTile = CurrentTile->GetTileNeighbours().UpNeighbour;
		break;
	case EFaceDirection::FaceDown:
		FacingTile = CurrentTile->GetTileNeighbours().DownNeighbour;
		break;
	case EFaceDirection::FaceLeft:
		FacingTile = CurrentTile->GetTileNeighbours().LeftNeighbour;
		break;
	case EFaceDirection::FaceRight:
		FacingTile = CurrentTile->GetTileNeighbours().RightNeighbour;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Failed to determine the direction we are facing"));
		break;
	}
	return FacingTile;
}

ATile* UInteractComponent::GetSecondTileInDirection(EFaceDirection& OutDirection)
{
	//First - Get the first tile(F) infront of the pawn (P)
	// |?|
	// |F|
	// |P|

	ATile* FirstFacingTile = GetTileInDirection(OutDirection);
	//Early return if FirstFacingTile is invalid
	if (!FirstFacingTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("No FirstFacingTile when trying to GetSecondTileInDirection()!"));
		return nullptr;
	}

	//Second - Get the next tile (S) using the first tile (F)
	// |S|
	// |F|
	// |P|

	ATile* SecondFacingTile = nullptr;

	//Determine which direction the pawn is facing
	EFaceDirection CurrentFacingDirection = PawnOwner->GetCurrentFaceDirection();
	OutDirection = CurrentFacingDirection;

	//Calculate the neighbouring tile in the direction we are facing
	switch (CurrentFacingDirection)
	{
	case EFaceDirection::FaceUp:
		SecondFacingTile = FirstFacingTile->GetTileNeighbours().UpNeighbour;
		break;
	case EFaceDirection::FaceDown:
		SecondFacingTile = FirstFacingTile->GetTileNeighbours().DownNeighbour;
		break;
	case EFaceDirection::FaceLeft:
		SecondFacingTile = FirstFacingTile->GetTileNeighbours().LeftNeighbour;
		break;
	case EFaceDirection::FaceRight:
		SecondFacingTile = FirstFacingTile->GetTileNeighbours().RightNeighbour;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Failed to determine the direction we are facing"));
		break;
	}

	return SecondFacingTile;
}

EMoveDirection UInteractComponent::DeterminePushDirection(EFaceDirection FaceDirection)
{
	EMoveDirection MoveDirection = EMoveDirection::None;
	switch (FaceDirection)
	{
	case EFaceDirection::FaceUp:
		MoveDirection = EMoveDirection::Up;
		break;
	case EFaceDirection::FaceDown:
		MoveDirection = EMoveDirection::Down;
		break;
	case EFaceDirection::FaceLeft:
		MoveDirection = EMoveDirection::Left;
		break;
	case EFaceDirection::FaceRight:
		MoveDirection = EMoveDirection::Right;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Failed to determine the direction to move in!"));
		break;
	}
	return MoveDirection;
}

EMoveDirection UInteractComponent::DeterminePullDirection(EFaceDirection FaceDirection)
{
	//When pulling the move direction is the inverse of the face direction
	//Facing Up = Moving Block Down
	//Facing Left = Moving Block to the Right
	EMoveDirection MoveDirection = EMoveDirection::None;
	switch (FaceDirection)
	{
	case EFaceDirection::FaceUp:
		MoveDirection = EMoveDirection::Down;
		break;
	case EFaceDirection::FaceDown:
		MoveDirection = EMoveDirection::Up;
		break;
	case EFaceDirection::FaceLeft:
		MoveDirection = EMoveDirection::Right;
		break;
	case EFaceDirection::FaceRight:
		MoveDirection = EMoveDirection::Left;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Failed to determine the direction to move in!"));
		break;
	}
	return MoveDirection;
}
