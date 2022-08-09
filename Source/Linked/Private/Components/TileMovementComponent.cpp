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
	PawnOwner->SetActorLocation(NewLocation);
}

// Called when the game starts
void UTileMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	//Get the pawn this component is attached to
	//PawnOwner = Cast<ALinkedPlayerPawn>(this->GetOwner());
	PawnOwner = this->GetOwner();

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

ATile* UTileMovementComponent::GetNeighbouringTileUp() const
{
	return CurrentTileNeighbours.UpNeighbour;
}

ATile* UTileMovementComponent::GetNeighbouringTileDown() const
{
	return CurrentTileNeighbours.DownNeighbour;
}

ATile* UTileMovementComponent::GetNeighbouringTileLeft() const
{
	return CurrentTileNeighbours.LeftNeighbour;
}

ATile* UTileMovementComponent::GetNeighbouringTileRight() const
{
	return CurrentTileNeighbours.RightNeighbour;
}

FTileNeighbours UTileMovementComponent::GetCurrentTileNeighbours() const
{
	return CurrentTileNeighbours;
}

bool UTileMovementComponent::CanMoveUp() const
{
	//Check up tile
	if (CurrentTileNeighbours.UpNeighbour == nullptr)
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
	if (CurrentTileNeighbours.DownNeighbour == nullptr)
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
	if (CurrentTileNeighbours.LeftNeighbour == nullptr)
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
	if (CurrentTileNeighbours.RightNeighbour == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void UTileMovementComponent::CheckVariables()
{
	//Check that we have a starting tile selected
	if (!PawnStartTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing PawnStartTile for: %s -> MovementComponent!"), *PawnOwner->GetActorNameOrLabel());
	}
	else
	{
		CurrentTile = PawnStartTile;
	}

	//Check to see if we have a Curve Float inserted in the editor
	if (!CurveFloat)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing a CurveFloat for: %s -> MovementComponent!"), *PawnOwner->GetActorNameOrLabel());
	}

	PawnOwner->SetActorLocation(CurrentTile->GetActorLocation());
}

void UTileMovementComponent::InitTimeline()
{
	UE_LOG(LogTemp, Warning, TEXT("InitTimeline"));
	IsMoving = true;

	FOnTimelineFloat TimelineProgress;
	TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
	CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
	CurveTimeline.SetLooping(false);
	CurveTimeline.PlayFromStart();
}

FOnTimelineEvent UTileMovementComponent::OnMoveCompleted()
{
	UE_LOG(LogTemp, Warning, TEXT("OnMoveCompleted"));

	FOnTimelineEvent Event;
	Event.BindUFunction(this, FName("UpdatePawnInformation"));
	return Event;
}

void UTileMovementComponent::UpdatePawnInformation()
{
	IsMoving = false;

	CurrentTile = TileMovedTo;
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
			UE_LOG(LogTemp, Warning, TEXT("Missing CurveFloat in MovementComponent!"));
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
			UE_LOG(LogTemp, Warning, TEXT("Missing CurveFloat in MovementComponent!"));
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
			UE_LOG(LogTemp, Warning, TEXT("Missing CurveFloat in MovementComponent!"));
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
			UE_LOG(LogTemp, Warning, TEXT("Missing CurveFloat in MovementComponent!"));
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

