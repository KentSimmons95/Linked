// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RuneKey.h"
#include "Actors/RuneExit.h"
#include "Player/LinkedPlayerPawn.h"
#include "Actors/Tile.h"

// Sets default values
ARuneKey::ARuneKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PickupRoot"));
	RootComponent = PickupRoot;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->AttachToComponent(PickupRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	OverlapBox->SetGenerateOverlapEvents(true);
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ARuneKey::OnPlayerEnterOverlapBox);
	OverlapBox->AttachToComponent(PickupRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

// Called when the game starts or when spawned
void ARuneKey::BeginPlay()
{
	Super::BeginPlay();

	if (!StartLocation)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartLocation missing for RuneKey Actor: %s"), *this->GetActorNameOrLabel());
	}
	else
	{
		this->SetActorLocation(StartLocation->GetActorLocation());
	}

	if (!RuneExit)
	{
		UE_LOG(LogTemp, Warning, TEXT("No RuneExit is associated with: %s"), *this->GetActorNameOrLabel());
	}
}

// Called every frame
void ARuneKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARuneKey::MoveToTileLocation()
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

void ARuneKey::OnPlayerEnterOverlapBox(UPrimitiveComponent* OverlappedActor, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!RuneExit)
	{
		UE_LOG(LogTemp, Warning, TEXT("No RuneExit is associated with: %s"), *this->GetActorNameOrLabel());
		return;
	}

	if (OtherActor->IsA(ALinkedPlayerPawn::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapped with: %s"), *this->GetActorNameOrLabel());
		UE_LOG(LogTemp, Warning, TEXT("%s is now unlocked"), *RuneExit->GetActorNameOrLabel());

		//Tell the RuneExit we have collected the key and then destroy the key actor
		RuneExit->KeyCollected();
		Destroy();
	}
}

