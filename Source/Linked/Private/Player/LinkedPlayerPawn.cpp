// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LinkedPlayerPawn.h"
#include "Player/LinkedPlayerController.h"

// Sets default values
ALinkedPlayerPawn::ALinkedPlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetCapsuleSize(36.0f, 100.0f);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->AttachToComponent(CapsuleComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ALinkedPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	SetupStartingPosition();
	GetPossibleMoves();

	PlayerController = Cast<ALinkedPlayerController>(GameplayStatics->GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		if (PlayerController->RegisterPlayerPawns(this))
		{
			UE_LOG(LogTemp, Warning, TEXT("Register to ALinkedPlayerController Successful!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Register to ALinkedPlayerController Failed!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to locate ALinkedPlayerController!"));
	}
	
}

// Called every frame
void ALinkedPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MovementDirection.IsZero())
	{
		const FVector NewLocation = GetActorLocation() + (MovementDirection * DeltaTime * MovementSpeed);
		SetActorLocation(NewLocation);
	}
}

// Called to bind functionality to input
void ALinkedPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALinkedPlayerPawn::MoveUp(float AxisValue)
{
	MovementDirection.X = FMath::Clamp(AxisValue, -1.0f, 1.0f);
}

void ALinkedPlayerPawn::MoveRight(float AxisValue)
{
	MovementDirection.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f);
}

ATile* ALinkedPlayerPawn::GetCurrentTile() const
{
	return CurrentTile;
}

bool ALinkedPlayerPawn::CanMoveUp(FTileNeighbours& Neighbours)
{
	//Check up tile
	if (Neighbours.UpNeighbour == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool ALinkedPlayerPawn::CanMoveDown(FTileNeighbours& Neighbours)
{
	//Check down tile
	if (Neighbours.DownNeighbour == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool ALinkedPlayerPawn::CanMoveLeft(FTileNeighbours& Neighbours)
{
	//Check left tile
	if (Neighbours.LeftNeighbour == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool ALinkedPlayerPawn::CanMoveRight(FTileNeighbours& Neighbours)
{
	//Check right tile
	if (Neighbours.RightNeighbour == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void ALinkedPlayerPawn::SetupStartingPosition()
{
	//Check to see if the pawn has a starting tile
	if (!PawnStartTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("No start tile selected for Pawn - %s"), *this->GetActorNameOrLabel());
		return;
	}

	CurrentTile = PawnStartTile;
	
	//Move the Pawn to its starting Tile
	this->SetActorLocation(PawnStartTile->GetActorLocation());
}

void ALinkedPlayerPawn::GetPossibleMoves()
{
	FTileNeighbours Neighbours = CurrentTile->GetTileNeighbours();

	// Check whether or not there is a neighour in each of the 4 possible directions 
	// If there is a tile - set flag to true or reset back to false if there isn't a tile
	
	PossibleMoves.bCanMoveUp = CanMoveUp(Neighbours);
	PossibleMoves.bCanMoveDown = CanMoveDown(Neighbours);
	PossibleMoves.bCanMoveLeft = CanMoveLeft(Neighbours);
	PossibleMoves.bCanMoveRight = CanMoveRight(Neighbours);
	
}

