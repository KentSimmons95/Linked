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

	if (!LinkedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing LinkedActor for %s LinkComponent!"), *ActorOwner->GetActorNameOrLabel());
	}
	else
	{
		CreateLinkWithActor();
	}
}


// Called every frame
void ULinkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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

	FVector CurrentLocation = ActorOwner->GetActorLocation();
	FRotator CurrentRotation = ActorOwner->GetActorRotation();
	FHitResult Hit;

	FVector Start = CurrentLocation;
	FVector End = LinkedActor->GetActorLocation();

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(ActorOwner);

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Pawn, TraceParams);

	HitActor = Hit.GetActor();

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);

	if (HitActor == LinkedActor)
	{
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(20, 20, 20), FColor::Blue, false, 2.0f);
		bIsCurrentlyLinked = true;
		//UE_LOG(LogTemp, Warning, TEXT("Line of sight with: %s!"), *LinkedActor->GetActorNameOrLabel());
		bHit = true;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("No line of sight with Actor: %s!"), *LinkedActor->GetActorNameOrLabel());
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

AActor* ULinkComponent::GetCurrentLinkedActor()
{
	return LinkedActor;
}

bool ULinkComponent::IsCurrentlyLinked()
{
	return bIsCurrentlyLinked;
}


