// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DirectionComponent.h"
#include "Player/LinkedPlayerPawn.h"

// Sets default values for this component's properties
UDirectionComponent::UDirectionComponent()
{

}


// Called when the game starts
void UDirectionComponent::BeginPlay()
{
	Super::BeginPlay();

	//Get the Owning Actor of this component
	PawnOwner = Cast<ALinkedPlayerPawn>(this->GetOwner());
	
	//Assert if the Cast was success
	checkf(PawnOwner, TEXT("Failed to Cast AActor to ALinkedPlayerPawn in DirectionComponent for %s"), *this->GetOwner()->GetActorNameOrLabel());
}

void UDirectionComponent::RotateActor(EFaceDirection Direction)
{
	//If   - the PawnOwner is still moving the return
	//Else - rotate the Actor Owner
	if (PawnOwner->IsPawnMoving())
	{
		return;
	}
	else
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
	
}

EFaceDirection UDirectionComponent::GetCurrentFaceDirection() const
{
	return CurrentFacingDirection;
}

void UDirectionComponent::FaceUp()
{
	if (CurrentFacingDirection != EFaceDirection::FaceUp)
	{
		PawnOwner->SetActorRotation(TurnUp);
		CurrentFacingDirection = EFaceDirection::FaceUp;
	}
}

void UDirectionComponent::FaceDown()
{
	if (CurrentFacingDirection != EFaceDirection::FaceDown)
	{
		PawnOwner->SetActorRotation(TurnDown);
		CurrentFacingDirection = EFaceDirection::FaceDown;
	}
}

void UDirectionComponent::FaceLeft()
{
	if (CurrentFacingDirection != EFaceDirection::FaceLeft)
	{
		PawnOwner->SetActorRotation(TurnLeft);
		CurrentFacingDirection = EFaceDirection::FaceLeft;
	}
}

void UDirectionComponent::FaceRight()
{
	if (CurrentFacingDirection != EFaceDirection::FaceRight)
	{
		PawnOwner->SetActorRotation(TurnRight);
		CurrentFacingDirection = EFaceDirection::FaceRight;
	}
}

