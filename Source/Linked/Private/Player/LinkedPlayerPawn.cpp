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

void ALinkedPlayerPawn::SetStartingPosition(FVector StartPosition)
{
	this->SetActorLocation(StartPosition);
}

void ALinkedPlayerPawn::Move(EMoveDirection Direction)
{
	TileMovementComponent->MoveToTile(Direction);
}
