// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"

//Forward Declaration
enum EFaceDirection;
enum EMoveDirection;
class ATile;
class ALinkedPlayerPawn;

//Interact Component that has the ability to interact with world actors such as blocks
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LINKED_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	
	//Temp for refactor - rename
	void TryPushBlock();
	void TryPullBlock();
	
private:
	ALinkedPlayerPawn* PawnOwner = nullptr;

	//Returns the tile that the pawn is currently facing towards
	ATile* GetTileInDirection(EFaceDirection& OutDirection);
	//Returns the second tile the pawn is currently facing towards (skips the first tile)
	ATile* GetSecondTileInDirection(EFaceDirection& OutDirection);
	//Get the MoveDirection to push a block using the Pawns FaceDirection
	EMoveDirection DeterminePushDirection(EFaceDirection FaceDirection);
	//Get the MoveDirection to pull a block using the Pawns FaceDirection
	EMoveDirection DeterminePullDirection(EFaceDirection FaceDirection);

};
