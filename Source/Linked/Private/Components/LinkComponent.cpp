// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LinkComponent.h"

// Sets default values for this component's properties
ULinkComponent::ULinkComponent()
{

}


// Called when the game starts
void ULinkComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ActorOwner = this->GetOwner();

	if (!LinkedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing LinkedActor for %s LinkComponent!"), *ActorOwner->GetActorNameOrLabel());
	}
	else
	{
		CreateLinkWithActor();
	}
}

//If the Pawns have direct line of sight of each other then they are considered linked
bool ULinkComponent::HasLineOfSight()
{
	checkf(LinkedActor, TEXT("ActorToLink is missing for Parent %s"), *ActorOwner->GetActorNameOrLabel());

	if (!LinkedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing LinkedActor for %s LinkComponent!"), *ActorOwner->GetActorNameOrLabel());
		return false;
	}

	bool bHit = false;

	//Get Owning actors transform as the origin for the line trace
	FVector CurrentLocation = ActorOwner->GetActorLocation();
	FRotator CurrentRotation = ActorOwner->GetActorRotation();

	//Store the FHitResult
	FHitResult Hit;

	FVector Start = CurrentLocation;
	FVector End = LinkedActor->GetActorLocation();

	FCollisionQueryParams TraceParams;

	//Ignore self in the FCollisionQueryParams
	TraceParams.AddIgnoredActor(ActorOwner);

	//Fire the line trace and set channel to Pawns
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Pawn, TraceParams);

	HitActor = Hit.GetActor();

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);

	if (HitActor == LinkedActor)
	{
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(20, 20, 20), FColor::Blue, false, 2.0f);
		bIsCurrentlyLinked = true;
		bHit = true;
	}
	else
	{
		bIsCurrentlyLinked = false;
		bHit = false;
	}

	return bHit;
}
void ULinkComponent::CreateLinkWithActor()
{
	if (!bIsCurrentlyLinked)
	{
		if (HasLineOfSight())
		{
			bIsCurrentlyLinked = true;
		}
		else
		{
			bIsCurrentlyLinked = false;
		}
	}
}

AActor* ULinkComponent::GetCurrentLinkedActor() const
{
	return LinkedActor;
}

bool ULinkComponent::IsCurrentlyLinked() const
{
	return bIsCurrentlyLinked;
}


