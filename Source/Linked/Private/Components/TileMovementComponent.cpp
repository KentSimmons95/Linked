// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TileMovementComponent.h"
#include "Player/LinkedPlayerPawn.h"

// Sets default values for this component's properties
UTileMovementComponent::UTileMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

void UTileMovementComponent::TimelineProgress(float Value)
{
	FVector NewLocation = FMath::Lerp(StartLoc, EndLoc, Value);
	ActorOwner->SetActorLocation(NewLocation);
}

// Called when the game starts
void UTileMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	//Get the pawn this component is attached to
	//PawnOwner = Cast<ALinkedPlayerPawn>(this->GetOwner());
	ActorOwner = this->GetOwner();

	CheckVariables();
	UpdateTileNeighbours();
	GetPossibleMoves();

	UE_LOG(LogTemp, Warning, TEXT("UpdatePawnInformation"));
	//Timeline OnCompleteFunction
	CurveTimeline.SetTimelineFinishedFunc(OnMoveCompleted());
}

// Called every frame
void UTileMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurveTimeline.TickTimeline(DeltaTime);
}

ATile* UTileMovementComponent::GetCurrentTile() const
{
	return CurrentTile;
}

void UTileMovementComponent::SetCurrentTile(ATile* NewTile)
{
	if (NewTile)
	{
		CurrentTile = NewTile;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot set CurrentTile to: %s"), *NewTile->GetActorNameOrLabel());
	}
}

void UTileMovementComponent::UpdateTileNeighbours()
{
	CurrentTileNeighbours = CurrentTile->GetTileNeighbours();
}

FTileNeighbours UTileMovementComponent::GetCurrentTileNeighbours() const
{
	return CurrentTileNeighbours;
}

bool UTileMovementComponent::HasMoveCompleted()
{
	//Return the NOT of IsMoving - if we are moving then the moving is NOT completed (false)
	return !IsMoving;
}

bool UTileMovementComponent::IsCurrentlyMoving()
{
	return IsMoving;
}

bool UTileMovementComponent::CanMoveUp() const
{
	//Readability
	ATile* UpNeightbour = CurrentTileNeighbours.UpNeighbour;

	//Check up tile
	if (UpNeightbour == nullptr || UpNeightbour->HasActorOnTile())
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
	//Readability
	ATile* DownNeighbour = CurrentTileNeighbours.DownNeighbour;

	//Check down tile
	if (DownNeighbour == nullptr || DownNeighbour->HasActorOnTile())
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
	//Readability
	ATile* LeftNeighbour = CurrentTileNeighbours.LeftNeighbour;

	//Check left tile
	if (LeftNeighbour == nullptr || LeftNeighbour->HasActorOnTile())
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
	//Readability
	ATile* RightNeighbour = CurrentTileNeighbours.RightNeighbour;

	//Check right tile
	if (RightNeighbour == nullptr || RightNeighbour->HasActorOnTile())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool UTileMovementComponent::CanMoveInDirection(EMoveDirection Direction)
{
	bool CanMove = false;

	switch (Direction)
	{
	case EMoveDirection::Up:
		CanMove = CanMoveUp();
		break;
	case EMoveDirection::Down:
		CanMove = CanMoveDown();
		break;
	case EMoveDirection::Left:
		CanMove = CanMoveLeft();
		break;
	case EMoveDirection::Right:
		CanMove = CanMoveRight();
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid move direction detected!"));
		break;
	}

	return CanMove;
}

void UTileMovementComponent::CheckVariables()
{
	//Check that we have a starting tile selected - then assign it as the CurrentTile
	if (!ActorStartTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActorStartTile has not been set for Actor - %s!"), *ActorOwner->GetActorNameOrLabel());
		return;
	}
	CurrentTile = ActorStartTile;
	CurrentTile->UpdateActorOnTile(ActorOwner);
	
	//Check to see if we have a Curve Float inserted in the editor
	if (!CurveFloat)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing a CurveFloat for: %s -> MovementComponent!"), *ActorOwner->GetActorNameOrLabel());
		return;
	}

	ActorOwner->SetActorLocation(CurrentTile->GetActorLocation());
}

void UTileMovementComponent::InitTimeline()
{
	IsMoving = true;

	FOnTimelineFloat TimelineProgress;
	TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
	CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
	CurveTimeline.SetLooping(false);
	CurveTimeline.PlayFromStart();
}

FOnTimelineEvent UTileMovementComponent::OnMoveCompleted()
{
	FOnTimelineEvent Event;
	Event.BindUFunction(this, FName("UpdateActorInformation"));
	return Event;
}

void UTileMovementComponent::UpdateActorInformation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnMoveCompleted"));

	IsMoving = false;
	
	//CurrentTile before it gets assigned to TileMovedTo (Tile moved from)
	CurrentTile->UpdateActorOnTile(nullptr);

	CurrentTile = TileMovedTo;

	//CurrentTile is now the current tile we are standing on (Tile moved to)
	CurrentTile->UpdateActorOnTile(ActorOwner);
	TileMovedTo = nullptr;

	SetCurrentTile(CurrentTile);
	UpdateTileNeighbours();

	GetPossibleMoves();
}

void UTileMovementComponent::MoveToTile(EMoveDirection Direction)
{
	switch (Direction)
	{
	case EMoveDirection::Up:
		MoveUp();
		break;

	case EMoveDirection::Down:
		MoveDown();
		break;

	case EMoveDirection::Left:
		MoveLeft();
		break;

	case EMoveDirection::Right:
		MoveRight();
		break;

	default:
		break;
	}
}

ATile* UTileMovementComponent::GetActorStartTile() const
{
	if (!ActorStartTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("Starting tile not set for this actor!"));
		return nullptr;
	}
	else
	{
		return ActorStartTile;
	}
}



void UTileMovementComponent::MoveUp()
{
	if (CanMoveUp() && !IsMoving)
	{
		//Check if there is a Curve Float in Editor
		if (CurveFloat)
		{
			InitTimeline();
			StartLoc = CurrentTile->GetActorLocation();
			EndLoc = CurrentTileNeighbours.UpNeighbour->GetActorLocation();
			
			TileMovedTo = CurrentTileNeighbours.UpNeighbour;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing CurveFloat in MovementComponent for Actor: %s"), *ActorOwner->GetActorNameOrLabel());
		}
	}
}

void UTileMovementComponent::MoveDown()
{
	if (CanMoveDown() && !IsMoving)
	{
		//Check if there is a Curve Float in Editor
		if (CurveFloat)
		{
			InitTimeline();
			StartLoc = CurrentTile->GetActorLocation();
			EndLoc = CurrentTileNeighbours.DownNeighbour->GetActorLocation();

			TileMovedTo = CurrentTileNeighbours.DownNeighbour;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing CurveFloat in MovementComponent for Actor: %s"), *ActorOwner->GetActorNameOrLabel());
		}
	}
}

void UTileMovementComponent::MoveLeft()
{
	if (CanMoveLeft() && !IsMoving)
	{
		//Check if there is a Curve Float in Editor
		if (CurveFloat)
		{
			InitTimeline();
			StartLoc = CurrentTile->GetActorLocation();
			EndLoc = CurrentTileNeighbours.LeftNeighbour->GetActorLocation();

			TileMovedTo = CurrentTileNeighbours.LeftNeighbour;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing CurveFloat in MovementComponent for Actor: %s"), *ActorOwner->GetActorNameOrLabel());
		}
	}
}

void UTileMovementComponent::MoveRight()
{
	if (CanMoveRight() && !IsMoving)
	{
		//Check if there is a Curve Float in Editor
		if (CurveFloat)
		{
			InitTimeline();
			StartLoc = CurrentTile->GetActorLocation();
			EndLoc = CurrentTileNeighbours.RightNeighbour->GetActorLocation();

			TileMovedTo = CurrentTileNeighbours.RightNeighbour;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing CurveFloat in MovementComponent for Actor: %s"), *ActorOwner->GetActorNameOrLabel());
		}
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

