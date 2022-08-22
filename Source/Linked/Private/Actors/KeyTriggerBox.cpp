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
	//Debugger helper for the trigger box
	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(20, 20, 20), FColor(0, 255,0), true);
}

void AKeyTriggerBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->IsA(ALinkedPlayerPawn::StaticClass()))
	{
		TriggeredByPlayer = true;
	}

	//After a player trigger the volume destroy the volume
	if (TriggeredByPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroying actor"));
		this->Destroy();
	}
}

