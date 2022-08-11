// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LinkedPlayerController.h"
#include "Player/LinkedPlayerPawn.h"

void ALinkedPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetupInput();
	UE_LOG(LogTemp, Warning, TEXT("Hello World"));
}

bool ALinkedPlayerController::RegisterPlayerPawns(ALinkedPlayerPawn* PlayerPawn)
{
	bool bRegisterSucces = false;
	
	if (PlayerPawn)
	{
		Pawns.Add(PlayerPawn);
		NumPawns++;

		FString PawnName = PlayerPawn->GetActorNameOrLabel();
		UE_LOG(LogTemp, Warning, TEXT("%s added to PlayerController"), *PawnName);

		bRegisterSucces = true;
		return bRegisterSucces;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to register ALinkedPlayerPawn!"));
		return bRegisterSucces;
	}
}

void ALinkedPlayerController::SetupInput()
{
	EnableInput(this);
	check(InputComponent);

	//Action Input Bindings
	InputComponent->BindAction("Up", EInputEvent::IE_Released, this, &ALinkedPlayerController::MoveUp);
	InputComponent->BindAction("Down", EInputEvent::IE_Released, this, &ALinkedPlayerController::MoveDown);
	InputComponent->BindAction("Left", EInputEvent::IE_Released, this, &ALinkedPlayerController::MoveLeft);
	InputComponent->BindAction("Right", EInputEvent::IE_Released, this, &ALinkedPlayerController::MoveRight);
	InputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &ALinkedPlayerController::Interact);
	InputComponent->BindAction("Link", EInputEvent::IE_Pressed, this, &ALinkedPlayerController::Toggle);
	
	

	//TODO Add interact bindings
}

bool ALinkedPlayerController::HasLineOfSight()
{
	bool bHit = false;
	
	FVector Location = Pawns[0]->GetActorLocation();
	FRotator Rotation = Pawns[0]->GetActorRotation();
	FHitResult Hit;

	FVector Start = Location;
	FVector End = Pawns[1]->GetActorLocation();

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(Pawns[0]);

	bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Pawn, TraceParams);

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);

	if (bHit)
	{
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(20, 20, 20), FColor::Blue, false, 2.0f);
		return true;
	}
	else
	{
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(20, 20, 20), FColor::Blue, false, 2.0f);
		return false;
	}
}

void ALinkedPlayerController::MoveUp()
{
	UE_LOG(LogTemp, Warning, TEXT("Up"));
	if (Pawns[0])
	{
		if (Pawns[0]->GetCurrentFaceDirection() != EFaceDirection::FaceUp)
		{
			Pawns[0]->Turn(EFaceDirection::FaceUp);
		}
		else
		{
			Pawns[0]->Move(EMoveDirection::Up);
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawns[0] not found!"));
	}
}

void ALinkedPlayerController::MoveDown()
{
	UE_LOG(LogTemp, Warning, TEXT("Down"));
	if (Pawns[0])
	{
		if (Pawns[0]->GetCurrentFaceDirection() != EFaceDirection::FaceDown)
		{
			Pawns[0]->Turn(EFaceDirection::FaceDown);
		}
		else
		{
			Pawns[0]->Move(EMoveDirection::Down);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawns[0] not found!"));
	}
}

void ALinkedPlayerController::MoveLeft()
{
	UE_LOG(LogTemp, Warning, TEXT("Left"));
	if (Pawns[0])
	{
		if (Pawns[0]->GetCurrentFaceDirection() != EFaceDirection::FaceLeft)
		{
			Pawns[0]->Turn(EFaceDirection::FaceLeft);
		}
		else
		{

			Pawns[0]->Move(EMoveDirection::Left);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawns[0] not found!"));
	}
}

void ALinkedPlayerController::MoveRight()
{
	UE_LOG(LogTemp, Warning, TEXT("Right"));
	if (Pawns[0])
	{
		if (Pawns[0]->GetCurrentFaceDirection() != EFaceDirection::FaceRight)
		{
			Pawns[0]->Turn(EFaceDirection::FaceRight);
		}
		else
		{
			Pawns[0]->Move(EMoveDirection::Right);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawns[0] not found!"));
	}
}

void ALinkedPlayerController::Interact()
{
	if (Pawns[0])
	{
		UE_LOG(LogTemp, Warning, TEXT("Interact button pressed!"));
		Pawns[0]->Interact();
	}
}

void ALinkedPlayerController::Toggle()
{
	Pawns[0]->StopNiagara();
}

