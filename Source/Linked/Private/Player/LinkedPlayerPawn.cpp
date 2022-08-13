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
	DirectionComponent = CreateDefaultSubobject<UDirectionComponent>(TEXT("DirectionComponent"));
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	LinkComponent = CreateDefaultSubobject<ULinkComponent>(TEXT("LinkComponent"));
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
}

// Called when the game starts or when spawned
void ALinkedPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	checkf(TileMovementComponent, TEXT("Failed to find TileMovementComponent on Actor - %s!"), *this->GetActorNameOrLabel());
	checkf(DirectionComponent, TEXT("Failed to find DirectionComponent on Actor - %s!"), *this->GetActorNameOrLabel());
	checkf(InteractComponent, TEXT("Failed to find InteractComponent on Actor - %s!"), *this->GetActorNameOrLabel());

	PlayerController = Cast<ALinkedPlayerController>(GameplayStatics->GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		PlayerController->RegisterPlayerPawns(this);
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

	if (!NiagaraComponent)
	{
		return;
	}

	UpdateLink();
}

// Called to bind functionality to input
void ALinkedPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALinkedPlayerPawn::SetStartingPosition(FVector StartPosition)
{
	this->SetActorLocation(StartPosition);
}

EFaceDirection ALinkedPlayerPawn::GetCurrentFaceDirection() const
{
	return DirectionComponent->GetCurrentFaceDirection();
}

void ALinkedPlayerPawn::Move(EMoveDirection Direction)
{
	TileMovementComponent->MoveToTile(Direction);
}

void ALinkedPlayerPawn::Turn(EFaceDirection Direction)
{
	DirectionComponent->RotateActor(Direction);
}

void ALinkedPlayerPawn::Interact()
{
	InteractComponent->Interact();
}

bool ALinkedPlayerPawn::LinkedStatus()
{
	return LinkComponent->IsCurrentlyLinked();
}

bool ALinkedPlayerPawn::CanMoveUp() const
{
	return TileMovementComponent->CanMoveUp();
}

bool ALinkedPlayerPawn::CanMoveDown() const
{
	return TileMovementComponent->CanMoveDown();
}

bool ALinkedPlayerPawn::CanMoveLeft() const
{
	return TileMovementComponent->CanMoveLeft();
}

bool ALinkedPlayerPawn::CanMoveRight() const
{
	return TileMovementComponent->CanMoveRight();
}

bool ALinkedPlayerPawn::CanMoveInDirection(EMoveDirection MoveDirection) const
{
	switch (MoveDirection)
	{
	case EMoveDirection::Up:
		return CanMoveUp();
		break;
	case EMoveDirection::Down:
		return CanMoveDown();
		break;
	case EMoveDirection::Left:
		return CanMoveLeft();
		break;
	case EMoveDirection::Right:
		return CanMoveRight();
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Failed to find a valid move direction for Pawn: %s!"), *this->GetActorNameOrLabel());
		return false;
	}
}

bool ALinkedPlayerPawn::IsFacingDirection(EFaceDirection Direction)
{
	if (DirectionComponent->GetCurrentFaceDirection() == Direction)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ALinkedPlayerPawn::IsPawnMoving()
{
	return TileMovementComponent->IsCurrentlyMoving();
}

void ALinkedPlayerPawn::UpdateLink()
{
	if (TileMovementComponent->HasMoveCompleted())
	{
		if (LinkComponent->HasLineOfSight())
		{
			NiagaraComponent->SetVisibility(true);
		}
		else
		{
			NiagaraComponent->SetVisibility(false);
		}
	}
}

void ALinkedPlayerPawn::SetActorStartLocation()
{
	if (TileMovementComponent->GetActorStartTile() != nullptr)
	{
		ATile* StartTile = TileMovementComponent->GetActorStartTile();
		this->SetActorLocation(StartTile->GetActorLocation()) ;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Starting tile not set on the TileMovementComponent for Actor: %s"), *this->GetActorNameOrLabel());
	}
}
