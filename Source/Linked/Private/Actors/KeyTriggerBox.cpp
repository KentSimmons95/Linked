// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/KeyTriggerBox.h"
#include "DrawDebugHelpers.h"
#include "Player/LinkedPlayerPawn.h"

AKeyTriggerBox::AKeyTriggerBox()
{

	OnActorBeginOverlap.AddDynamic(this, &AKeyTriggerBox::OnOverlapBegin);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AKeyTriggerBox::BeginPlay()
{
	//Debug of the trigger box for testing
	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(20, 20, 20), FColor(0, 255,0), true);
}

void AKeyTriggerBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->IsA(ALinkedPlayerPawn::StaticClass()))
	{
		//Update Game mode/level manager to unlock door

		//Set triggered by player to true
		TriggeredByPlayer = true;
	}

	//After a player trigger the volume destroy the volume
	if (TriggeredByPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroying actor"));
		this->Destroy();
	}
}

