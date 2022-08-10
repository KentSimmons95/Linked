// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractComponent.h"
#include "Player/LinkedPlayerPawn.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	//Try in Constructor?
	ActorOwner = Cast<ALinkedPlayerPawn>(this-> GetOwner());
}

void UInteractComponent::Interact()
{
	LineTrace();
}

void UInteractComponent::LineTrace()
{
	bool bHit = false;

	CurrentLocation = ActorOwner->GetActorLocation();
	CurrentRotation = ActorOwner->GetActorRotation();
	FHitResult Hit;

	FVector Start = CurrentLocation;
	FVector End = CalculateEndLocation();

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

FVector UInteractComponent::CalculateEndLocation()
{
	FVector TraceEndLocation = CurrentLocation;
	int32 YawRotation = CurrentRotation.Yaw;

	UE_LOG(LogTemp, Warning, TEXT("Actors current yaw rotation is: %i!"), YawRotation);

	switch (YawRotation)
	{
	case 0:
		//Shoot up (+100 on X Axis)
		TraceEndLocation.X += LineTraceLength;
		break;
	case 180:
		//Shoot down (-100 on X Axis)
		TraceEndLocation.X -= LineTraceLength;
		break;
	case -90:
		//Shoot left (-100 on Y Axis)
		TraceEndLocation.Y -= LineTraceLength;
		break;
	//TODO - look into why it Currently sets to 89 (somehow when rotating the Actor to the right it gets set to 89 instead of 90)
	case 89:
		//Shoot right (+100 on Y Axis)
		TraceEndLocation.Y += LineTraceLength;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unable to determine actors direction!"));
	}

	return TraceEndLocation;
}
