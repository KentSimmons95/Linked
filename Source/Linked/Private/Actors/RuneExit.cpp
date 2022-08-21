// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RuneExit.h"
#include "Actors/Tile.h"
#include "Player/LinkedPlayerPawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARuneExit::ARuneExit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PickupRoot"));
	RootComponent = PickupRoot;

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	OverlapBox->SetGenerateOverlapEvents(false);
	OverlapBox->AttachToComponent(PickupRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ARuneExit::OnPlayerEnterOverlapBox);
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &ARuneExit::OnPlayerLeaveOverlapBox);
	
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->AttachToComponent(PickupRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

// Called when the game starts or when spawned
void ARuneExit::BeginPlay()
{
	Super::BeginPlay();

	NiagaraComponent->SetVisibility(false);

	UGameplayStatics* GameplayStatics;
	AGameModeBase* GameMode = GameplayStatics->GetGameMode(GetWorld());

	//Check if we can find the correct game mode
	if (GameMode->IsA(ALinkedGameMode::StaticClass()))
	{
		LinkedGameMode = Cast<ALinkedGameMode>(GameMode);
		//Register the RuneExit to the GameMode
		if (LinkedGameMode->RegisterRuneExit(this))
		{
			UE_LOG(LogTemp, Warning, TEXT("Registered %s to LinkedGameMode!"), *this->GetActorNameOrLabel());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to register %s to LinkedGameMode!"), *this->GetActorNameOrLabel());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Game Mode is not a LinkedGameMode!"));
	}

	//Check if we have a TileStartLocation so we can use its starting location for this actors location
	if (!StartLocation)
	{
		UE_LOG(LogTemp, Warning, TEXT("TileStartLocation is missing for: %s"), *this->GetActorNameOrLabel());
	}
	else
	{
		this->SetActorLocation(StartLocation->GetActorLocation());
	}
}

void ARuneExit::KeyCollected()
{
	UE_LOG(LogTemp, Warning, TEXT("Enabled RuneExit!"));
	OverlapBox->SetGenerateOverlapEvents(true);
	NiagaraComponent->SetVisibility(true);
}

bool ARuneExit::IsPawnStandingOnExit() const
{
	return bIsPawnStandingOnExit;
}

void ARuneExit::MoveToTileLocation()
{
	if (!StartLocation)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to move RuneKey actor because Start Location is empty!"));
	}
	else
	{
		this->SetActorLocation(StartLocation->GetActorLocation());
	}
}

bool ARuneExit::IsAPlayerPawn(AActor* OtherActor)
{
	if (OtherActor->IsA(ALinkedPlayerPawn::StaticClass()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ARuneExit::OnPlayerEnterOverlapBox(UPrimitiveComponent* OverlappedActor, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped with: %s"), *this->GetActorNameOrLabel());

	//Test if OtherActor is a PlayerPawn
	//Tell game mode that a player is standing on exit 
	//Test if both Pawns are on the exit
	if (IsAPlayerPawn(OtherActor))
	{
		bIsPawnStandingOnExit = true;
		LinkedGameMode->CheckIfEndOfLevel();
	}
}

void ARuneExit::OnPlayerLeaveOverlapBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Tell game mode player is no longer standing on exit - disable flag
	if (IsAPlayerPawn(OtherActor))
	{
		bIsPawnStandingOnExit = false;
	}
}

