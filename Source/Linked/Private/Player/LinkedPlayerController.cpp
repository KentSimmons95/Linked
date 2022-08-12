// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LinkedPlayerController.h"
#include "Player/LinkedPlayerPawn.h"

void ALinkedPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetupInput();
	UE_LOG(LogTemp, Warning, TEXT("Hello World"));
}

void ALinkedPlayerController::RegisterPlayerPawns(ALinkedPlayerPawn* PlayerPawn)
{
	if (PlayerPawn->ActorHasTag("LeftPawn"))
	{
		LeftPawn = PlayerPawn;
		LeftPawnRegistered = true;
		UE_LOG(LogTemp, Warning, TEXT("%s successfully added to PlayerController"), *LeftPawn->GetActorNameOrLabel());
	}
	else if (PlayerPawn->ActorHasTag("RightPawn"))
	{
		RightPawn = PlayerPawn;
		RightPawnRegistered = true;
		UE_LOG(LogTemp, Warning, TEXT("%s successfully added to PlayerController"), *RightPawn->GetActorNameOrLabel());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s failed to register to PlayerController"), *PlayerPawn->GetActorNameOrLabel());
	}
}

void ALinkedPlayerController::SetupInput()
{
	EnableInput(this);
	check(InputComponent);

	//Action Input Bindings for LeftPawn
	InputComponent->BindAction("LeftPawnUp", EInputEvent::IE_Released, this, &ALinkedPlayerController::LeftPawnMoveUp);
	InputComponent->BindAction("LeftPawnDown", EInputEvent::IE_Released, this, &ALinkedPlayerController::LeftPawnMoveDown);
	InputComponent->BindAction("LeftPawnLeft", EInputEvent::IE_Released, this, &ALinkedPlayerController::LeftPawnMoveLeft);
	InputComponent->BindAction("LeftPawnRight", EInputEvent::IE_Released, this, &ALinkedPlayerController::LeftPawnMoveRight);

	//Action Input Bindings for RightPawn
	InputComponent->BindAction("RightPawnUp", EInputEvent::IE_Released, this, &ALinkedPlayerController::RightPawnMoveUp);
	InputComponent->BindAction("RightPawnDown", EInputEvent::IE_Released, this, &ALinkedPlayerController::RightPawnMoveDown);
	InputComponent->BindAction("RightPawnLeft", EInputEvent::IE_Released, this, &ALinkedPlayerController::RightPawnMoveLeft);
	InputComponent->BindAction("RightPawnRight", EInputEvent::IE_Released, this, &ALinkedPlayerController::RightPawnMoveRight);

	//Add interact bindings
	InputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &ALinkedPlayerController::Interact);
}


void ALinkedPlayerController::LeftPawnMoveUp()
{
	if (!LeftPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to move LeftPawn up because LeftPawn is invalid!"));
		return;
	}
	else
	{
		//If both left and right pawn are facing up then move
		//Else rotate the pawns to face up before moving
		if(LeftPawn->IsFacingDirection(EFaceDirection::FaceUp) && RightPawn->IsFacingDirection(EFaceDirection::FaceUp))
		{
			//Then check that both pawns can move up before moving
			if (LeftPawn->CanMoveUp() && RightPawn->CanMoveUp())
			{
				LeftPawn->Move(EMoveDirection::Up);
				RightPawn->Move(EMoveDirection::Up);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to move up!"));
			}
		}
		else
		{
			LeftPawn->Turn(EFaceDirection::FaceUp);
			RightPawn->Turn(EFaceDirection::FaceUp);
		}
	}
}

void ALinkedPlayerController::LeftPawnMoveDown()
{
	if (!LeftPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to move LeftPawn up because LeftPawn is invalid!"));
		return;
	}
	else
	{
		//If both left and right pawn are facing up then move
		//Else rotate the pawns to face up before moving
		if (LeftPawn->IsFacingDirection(EFaceDirection::FaceDown) && RightPawn->IsFacingDirection(EFaceDirection::FaceDown))
		{
			//Then check that both pawns can move up before moving
			if (LeftPawn->CanMoveDown() && RightPawn->CanMoveDown())
			{
				LeftPawn->Move(EMoveDirection::Down);
				RightPawn->Move(EMoveDirection::Down);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to move down!"));
			}
		}
		else
		{
			LeftPawn->Turn(EFaceDirection::FaceDown);
			RightPawn->Turn(EFaceDirection::FaceDown);
		}
	}
}
/*
void ALinkedPlayerController::LeftPawnMoveLeft()
{
}
void ALinkedPlayerController::LeftPawnMoveRight()
{
}
*/
void ALinkedPlayerController::LeftPawnMoveLeft()
{
	if (!LeftPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to move LeftPawn up because LeftPawn is invalid!"));
		return;
	}
	else
	{
		//If we are currently linked then boths pawns will move left (if they can)
		if (LeftPawn->LinkedStatus())
		{
			if (IsFacingSameDirection(EFaceDirection::FaceLeft))
			{
				if (CanBothMoveInDirection(EMoveDirection::Left))
				{
					MoveBothInDirection(EMoveDirection::Left);
				}
			}
			else
			{
				TurnSameDirection(EFaceDirection::FaceLeft);
			}
		}
		else
		{
			//The pawns aren't linked - so we are moving just the left pawn to the left
			if (LeftPawn->IsFacingDirection(EFaceDirection::FaceLeft) && LeftPawn->CanMoveLeft())
			{
				LeftPawn->Move(EMoveDirection::Left);
			}
			else
			{
				LeftPawn->Turn(EFaceDirection::FaceLeft);
			}
		}
	}
}

void ALinkedPlayerController::LeftPawnMoveRight()
{
	if (!LeftPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to move LeftPawn up because LeftPawn is invalid!"));
		return;
	}
	else
	{
		//If we are currently linked then boths pawns will move left (if they can)
		if (LeftPawn->LinkedStatus())
		{
			if (IsFacingSameDirection(EFaceDirection::FaceRight))
			{
				if (CanBothMoveInDirection(EMoveDirection::Right))
				{
					MoveBothInDirection(EMoveDirection::Right);
				}
			}
			else
			{
				TurnSameDirection(EFaceDirection::FaceRight);
			}
		}
		else
		{
			//The pawns aren't linked - so we are moving just the left pawn to the right
			if (LeftPawn->IsFacingDirection(EFaceDirection::FaceRight) && LeftPawn->CanMoveRight())
			{
				LeftPawn->Move(EMoveDirection::Right);
			}
			else
			{
				LeftPawn->Turn(EFaceDirection::FaceRight);
			}
		}
	}
}

void ALinkedPlayerController::RightPawnMoveUp()
{
	if (RightPawn)
	{
		if (RightPawn->GetCurrentFaceDirection() != EFaceDirection::FaceUp)
		{
			RightPawn->Turn(EFaceDirection::FaceUp);
		}
		else
		{
			RightPawn->Move(EMoveDirection::Up);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to move RightPawn up!"));
	}
}

void ALinkedPlayerController::RightPawnMoveDown()
{
	if (RightPawn)
	{
		if (RightPawn->GetCurrentFaceDirection() != EFaceDirection::FaceDown)
		{
			RightPawn->Turn(EFaceDirection::FaceDown);
		}
		else
		{
			RightPawn->Move(EMoveDirection::Down);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to move RightPawn down!"));
	}
}

void ALinkedPlayerController::RightPawnMoveLeft()
{
	if (RightPawn)
	{
		if (RightPawn->GetCurrentFaceDirection() != EFaceDirection::FaceLeft)
		{
			RightPawn->Turn(EFaceDirection::FaceLeft);
		}
		else
		{

			RightPawn->Move(EMoveDirection::Left);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to move RightPawn left!"));
	}
}

void ALinkedPlayerController::RightPawnMoveRight()
{
	if (RightPawn)
	{
		if (RightPawn->GetCurrentFaceDirection() != EFaceDirection::FaceRight)
		{
			RightPawn->Turn(EFaceDirection::FaceRight);
		}
		else
		{
			RightPawn->Move(EMoveDirection::Right);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to move RightPawn right!"));
	}
}
bool ALinkedPlayerController::IsFacingSameDirection(EFaceDirection FaceDirection)
{
	if (LeftPawn->IsFacingDirection(FaceDirection) && RightPawn->IsFacingDirection(FaceDirection))
	{
		return true;
	}
	else
	{
		return false;
	}
}
void ALinkedPlayerController::TurnSameDirection(EFaceDirection FaceDirection)
{
	LeftPawn->Turn(FaceDirection);
	RightPawn->Turn(FaceDirection);
}
bool ALinkedPlayerController::CanBothMoveInDirection(EMoveDirection MoveDirection)
{
	if (LeftPawn->CanMoveInDirection(MoveDirection) && RightPawn->CanMoveInDirection(MoveDirection))
	{
		return true;
	}
	else
	{
		return false;
	}
}
void ALinkedPlayerController::MoveBothInDirection(EMoveDirection MoveDirection)
{
	LeftPawn->Move(MoveDirection);
	RightPawn->Move(MoveDirection);
}
/*
bool ALinkedPlayerController::IsFacingSameDirection(EFaceDirection FaceDirection)
{
	if (LeftPawn->IsFacingDirection(FaceDirection) && RightPawn->IsFacingDirection(FaceDirection))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ALinkedPlayerController::TurnSameDirection(EFaceDirection FaceDirection)
{
	LeftPawn->Turn(FaceDirection);
	RightPawn->Turn(FaceDirection);
}

bool ALinkedPlayerController::CanBothMoveInDirection(EMoveDirection MoveDirection)
{
	if (LeftPawn->CanMoveInDirection(MoveDirection) && RightPawn->CanMoveInDirection(MoveDirection))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ALinkedPlayerController::MoveBothInDirection(EMoveDirection MoveDirection)
{
	LeftPawn->Move(MoveDirection);
	RightPawn->Move(MoveDirection);
}
*/

void ALinkedPlayerController::Interact()
{
	if (LeftPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interact button pressed!"));
		LeftPawn->Interact();
	}
}

