// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LinkComponent.h"

// Sets default values for this component's properties
ULinkComponent::ULinkComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULinkComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ActorOwner = this->GetOwner();
}


// Called every frame
void ULinkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsCurrentlyLinked)
	{
		if (!LinkedActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("LinkedActor not found!"));
			return;
		}
		else
		{
			SpawnLinkParticle();
			UE_LOG(LogTemp, Warning, TEXT("Linked!"));
		}
	}
}

void ULinkComponent::CreateLinkWithActor(AActor* ActorToLink)
{
	if (bIsCurrentlyLinked)
	{
		PreviousLinkedActor = LinkedActor;
		//Break link with existing link - then establish a new link 
		BreakCurrentLinkWithActor();
		if (ActorToLink == PreviousLinkedActor)
		{
			LinkedActor = nullptr;
			PreviousLinkedActor = nullptr;
			bIsCurrentlyLinked = false;
			return;
		}
		/*
		else
		{
			LinkedActor = ActorToLink;
			bIsCurrentlyLinked = true;
		}
		*/
	}
	else
	{
		LinkedActor = ActorToLink;
		bIsCurrentlyLinked = true;
	}
}

void ULinkComponent::BreakCurrentLinkWithActor()
{
	if (LinkedActor != nullptr)
	{
		LinkedActor = nullptr;
		bIsCurrentlyLinked = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Currently not linked to any actor!"));
	}
}

void ULinkComponent::SpawnLinkParticle()
{
	bool bHit = false;

	FVector CurrentLocation = ActorOwner->GetActorLocation();
	FRotator CurrentRotation = ActorOwner->GetActorRotation();
	FHitResult Hit;

	FVector Start = CurrentLocation;
	FVector End = LinkedActor->GetActorLocation();

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(ActorOwner);

	bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Pawn, TraceParams);

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);

	if (bHit)
	{
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(20, 20, 20), FColor::Blue, false, 2.0f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Actors hit!"));
	}
}

AActor* ULinkComponent::GetCurrentLinkedActor()
{
	return LinkedActor;
}

bool ULinkComponent::IsCurrentlyLinked()
{
	return bIsCurrentlyLinked;
}

