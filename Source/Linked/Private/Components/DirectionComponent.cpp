// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DirectionComponent.h"

// Sets default values for this component's properties
UDirectionComponent::UDirectionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDirectionComponent::BeginPlay()
{
	Super::BeginPlay();

	//Get the Owning Actor of this component
	ActorOwner = this->GetOwner();
}


void UDirectionComponent::RotateActor(EFaceDirection Direction)
{
	switch (Direction)
	{
	case EFaceDirection::FaceUp:
		FaceUp();
		break;

	case EFaceDirection::FaceDown:
		FaceDown();
		break;

	case EFaceDirection::FaceLeft:
		FaceLeft();
		break;

	case EFaceDirection::FaceRight:
		FaceRight();
		break;

	default:
		break;
	}
}

EFaceDirection UDirectionComponent::GetCurrentFaceDirection()
{
	return CurrentFacingDirection;
}

void UDirectionComponent::FaceUp()
{
	if (CurrentFacingDirection != EFaceDirection::FaceUp)
	{
		ActorOwner->SetActorRotation(TurnUp);
		CurrentFacingDirection = EFaceDirection::FaceUp;
	}
}

void UDirectionComponent::FaceDown()
{
	if (CurrentFacingDirection != EFaceDirection::FaceDown)
	{
		ActorOwner->SetActorRotation(TurnDown);
		CurrentFacingDirection = EFaceDirection::FaceDown;
	}
}

void UDirectionComponent::FaceLeft()
{
	if (CurrentFacingDirection != EFaceDirection::FaceLeft)
	{
		ActorOwner->SetActorRotation(TurnLeft);
		CurrentFacingDirection = EFaceDirection::FaceLeft;
	}
}

void UDirectionComponent::FaceRight()
{
	if (CurrentFacingDirection != EFaceDirection::FaceRight)
	{
		ActorOwner->SetActorRotation(TurnRight);
		CurrentFacingDirection = EFaceDirection::FaceRight;
	}
}

