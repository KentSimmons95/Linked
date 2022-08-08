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

	//Axis Input Bindings
	//InputComponent->BindAxis("MoveUp", this, &ALinkedPlayerController::MoveUp);
	//InputComponent->BindAxis("MoveWatsonRight", this, &ALinkedPlayerController::MoveWatsonRight);
	//InputComponent->BindAxis("MoveSherlockRight", this, &ALinkedPlayerController::MoveSherlockRight);

	//Action Input Bindings
	InputComponent->BindAction("Up", EInputEvent::IE_Released, this, &ALinkedPlayerController::MoveUp);
	InputComponent->BindAction("Down", EInputEvent::IE_Released, this, &ALinkedPlayerController::MoveDown);
	InputComponent->BindAction("Left", EInputEvent::IE_Released, this, &ALinkedPlayerController::MoveLeft);
	InputComponent->BindAction("Right", EInputEvent::IE_Released, this, &ALinkedPlayerController::MoveRight);
}

/*
void ALinkedPlayerController::MoveUp(float AxisValue)
{
	for (int i = 0; i < NumPawns; i++)
	{
		//Pawns[i]->MoveUp(AxisValue);
	}
}

void ALinkedPlayerController::MoveWatsonRight(float AxisValue)
{
	if (HasLineOfSight())
	{
		
		UE_LOG(LogTemp, Warning, TEXT("Watson"));
		for (int i = 0; i < NumPawns; i++)
		{
			//Pawns[i]->MoveRight(AxisValue);
		}
	}
	else
	{
		//Pawns[0]->MoveRight(AxisValue);
	}
}

void ALinkedPlayerController::MoveSherlockRight(float AxisValue)
{
	if (HasLineOfSight())
	{
		UE_LOG(LogTemp, Warning, TEXT("Sherlock"));
		for (int i = 0; i < NumPawns; i++)
		{
			//Pawns[i]->MoveRight(AxisValue);
		}
	}
	else
	{
		//Pawns[1]->MoveRight(AxisValue);
	}
}
*/

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
		Pawns[0]->MoveUp();
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

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawns[0] not found!"));
	}
}
