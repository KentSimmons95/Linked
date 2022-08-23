// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LinkedPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "World/LinkedGameMode.h"
#include "Player/LinkedPlayerPawn.h"

void ALinkedPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetupInput();
}

void ALinkedPlayerController::RegisterPlayerPawns(ALinkedPlayerPawn* PlayerPawn)
{
	//Register the Player Pawns to the controller based on their Actor Tags
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

int32 ALinkedPlayerController::GetNumberOfMovesRemaining() const
{
	return NumberOfMovesRemaining;
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
	InputComponent->BindAction("PawnPush", EInputEvent::IE_Pressed, this, &ALinkedPlayerController::PawnPush);
	InputComponent->BindAction("PawnPull", EInputEvent::IE_Pressed, this, &ALinkedPlayerController::PawnPull);
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
			//Then check that both pawns can move up before moving and that there are available move points
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
		if (LeftPawn->GetLinkedStatus())
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
		if (LeftPawn->GetLinkedStatus())
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

//Could just use LeftPawnMoveUp() - leaving it here incase RightPawn movement can do more
void ALinkedPlayerController::RightPawnMoveUp()
{
	if (!RightPawn || !LeftPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to move RightPawn up because the RightPawn OR LeftPawn are invalid!"));
		return;
	}
	else
	{
		//If both left and right pawn are facing up then move
		//Else rotate the pawns to face up before moving
		if (LeftPawn->IsFacingDirection(EFaceDirection::FaceUp) && RightPawn->IsFacingDirection(EFaceDirection::FaceUp))
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

//Could just use LeftPawnMoveUp() - leaving it here incase RightPawn movement can do more
void ALinkedPlayerController::RightPawnMoveDown()
{
	if (!RightPawn || !LeftPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to move RightPawn down because the RightPawn OR LeftPawn are invalid!"));
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

//Could just use LeftPawnMoveLeft() - leaving it here incase RightPawn movement can do more
void ALinkedPlayerController::RightPawnMoveLeft()
{
	if (!RightPawn || !LeftPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to move RightPawn left because eitehr RightPawn OR LeftPawn is invalid!"));
		return;
	}
	else
	{
		//If we are currently linked then boths pawns will move left (if they can)
		if (RightPawn->GetLinkedStatus())
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
			if (RightPawn->IsFacingDirection(EFaceDirection::FaceLeft) && RightPawn->CanMoveLeft())
			{
				RightPawn->Move(EMoveDirection::Left);
			}
			else
			{
				RightPawn->Turn(EFaceDirection::FaceLeft);
			}
		}
	}
}

//Could just use LeftPawnMoveRight() - leaving it here incase RightPawn movement can do more
void ALinkedPlayerController::RightPawnMoveRight()
{
	if (!RightPawn || !LeftPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to move RightPawn right because either RightPawn OR LeftPawn is invalid!"));
		return;
	}
	else
	{
		//If we are currently linked then boths pawns will move left (if they can)
		if (RightPawn->GetLinkedStatus())
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
			//The pawns aren't linked - so we are moving just the left pawn to the left
			if (RightPawn->IsFacingDirection(EFaceDirection::FaceRight) && RightPawn->CanMoveRight())
			{
				RightPawn->Move(EMoveDirection::Right);
			}
			else
			{
				RightPawn->Turn(EFaceDirection::FaceRight);
			}
		}
	}
}

void ALinkedPlayerController::PawnPush()
{
	/*Test if there is a block in front of the left pawn
	 *If there is then push the block 1 tile away from the left pawn
	 */
	if (LeftPawn->GetLinkedStatus())
	{
		LeftPawn->PushBlock();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("We cant push the block because we arent linked!"));
	}
}

void ALinkedPlayerController::PawnPull()
{
	/*If there is an empty tile followed by a block 
	 *actor immediately after the right pawn
	 *then pull the block onto the empty tile space
	*/
	UE_LOG(LogTemp, Warning, TEXT("Trying to pull!"));

	if (RightPawn->GetLinkedStatus())
	{
		UE_LOG(LogTemp, Warning, TEXT("We are linked!"));
		RightPawn->PullBlock();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("We cant pull the block because we arent linked!"));
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


