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
	PawnOwner = Cast<ALinkedPlayerPawn>(this->GetOwner());
	CurrentTile = PawnOwner->GetCurrentTile();
	GetPossibleMoves();

	UE_LOG(LogTemp, Warning, TEXT("Hi!"));
	//Timeline OnCompleteFunction
	CurveTimeline.SetTimelineFinishedFunc(OnMoveCompleted());
}


// Called every frame
void UTileMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurveTimeline.TickTimeline(DeltaTime);
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

FOnTimelineEvent UTileMovementComponent::OnMoveCompleted()
{
	FOnTimelineEvent Event;
	Event.BindUFunction(this, FName("UpdatePawnInformation"));
	return Event;
}

void UTileMovementComponent::UpdatePawnInformation()
{
	UE_LOG(LogTemp, Warning, TEXT("Hi!"));

	CurrentTile = TileMovedTo;
	TileMovedTo = nullptr;
	PawnOwner->SetCurrentTile(CurrentTile);
	PawnOwner->UpdateTileNeighbours();
	GetPossibleMoves();
}

void UTileMovementComponent::MoveToTile(float DeltaTime, ATile* Tile)
{
	PawnOwner->SetActorLocation(FMath::VInterpConstantTo(
		PawnOwner->GetActorLocation(),
		Tile->GetActorLocation(),
		DeltaTime,
		InterpSpeed));
}

void UTileMovementComponent::MoveUp()
{
	//Check if the move is possible to make
	GetPossibleMoves();

	if (CanMoveUp())
	{
		if (CurveFloat)
		{
			FOnTimelineFloat TimelineProgress;
			TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
			CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
			CurveTimeline.SetLooping(false);

			StartLoc = PawnOwner->GetCurrentTile()->GetActorLocation();
			EndLoc = PawnOwner->GetNeighbouringTileUp()->GetActorLocation();
			
			CurveTimeline.PlayFromStart();

			//Update the Tile we moved to - used to update the PlayerPawn's new Tile
			TileMovedTo = PawnOwner->GetNeighbouringTileUp();
			
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing CurveFloat in MovementComponent!"));

		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to MoveUp"));
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

