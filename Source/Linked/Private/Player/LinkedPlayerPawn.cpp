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

	UpdateLink();

	if (!NiagaraComponent)
	{
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NiagaraSystem attached on: %s"), *this->GetActorNameOrLabel());
	}
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

EFaceDirection ALinkedPlayerPawn::GetCurrentFaceDirection()
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

void ALinkedPlayerPawn::UpdateLink()
{
	if (TileMovementComponent->HasMoveCompleted())
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s HasMoveCompleted = TRUE"), *this->GetActorNameOrLabel());
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s HasMoveCompleted = FALSE"), *this->GetActorNameOrLabel());
	}
}

void ALinkedPlayerPawn::StopNiagara()
{
	NiagaraComponent->SetVisibility(false);
}