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

	TileMovementComponent = CreateDefaultSubobject<UTileMovementComponent>(TEXT("TileMovementComponent"));
}

// Called when the game starts or when spawned
void ALinkedPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	SetupStartingPosition();
	CurrentTileNeighbours = CurrentTile->GetTileNeighbours();

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

ATile* ALinkedPlayerPawn::GetCurrentTile() const
{
	return CurrentTile;
}

void ALinkedPlayerPawn::SetCurrentTile(ATile* NewTile)
{
	if (NewTile)
	{
		CurrentTile = NewTile;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot set %s as a new tile for %s!"), *NewTile->GetActorNameOrLabel(), *this->GetActorNameOrLabel());
	}
}

void ALinkedPlayerPawn::UpdateTileNeighbours()
{
	CurrentTileNeighbours = CurrentTile->GetTileNeighbours();
}

ATile* ALinkedPlayerPawn::GetNeighbouringTileUp() const
{
	return CurrentTileNeighbours.UpNeighbour;
}

ATile* ALinkedPlayerPawn::GetNeighbouringTileDown() const
{
	return CurrentTileNeighbours.DownNeighbour;
}

ATile* ALinkedPlayerPawn::GetNeighbouringTileLeft() const
{
	return CurrentTileNeighbours.LeftNeighbour;
}

ATile* ALinkedPlayerPawn::GetNeighbouringTileRight() const
{
	return CurrentTileNeighbours.RightNeighbour;
}

FTileNeighbours ALinkedPlayerPawn::GetCurrentTileNeighbours() const
{
	return CurrentTileNeighbours;
}

void ALinkedPlayerPawn::MoveUp()
{
	if(TileMovementComponent)
	{
		//TileMovementComponent->MoveUp();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TileMovementComponent not found!"));
	}
}

void ALinkedPlayerPawn::MoveDown()
{
}

void ALinkedPlayerPawn::MoveLeft()
{
}

void ALinkedPlayerPawn::MoveRight()
{
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

