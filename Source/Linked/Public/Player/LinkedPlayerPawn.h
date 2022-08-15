// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Actors/Tile.h"
#include "Components/TileMovementComponent.h"
#include "Components/LinkComponent.h"
#include "Components/DirectionComponent.h"
#include "Components/InteractComponent.h"
#include "GameplayTagContainer.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "LinkedPlayerPawn.generated.h"

class ALinkedPlayerController;

UCLASS()
class LINKED_API ALinkedPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALinkedPlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetStartingPosition(FVector StartPosition);
	EFaceDirection GetCurrentFaceDirection() const;

	//Get whether we are facing a certain direction
	bool IsFacingDirection(EFaceDirection Direction);
	//Get whether the pawn is moving or not
	bool IsPawnMoving();

	//Calls the TileMovementComponent that is attached to the Pawn to move tile
	void Move(EMoveDirection Direction);
	void Turn(EFaceDirection Direction);

	void PushBlock();
	void PullBlock();

	//The current link status of the Pawn
	bool LinkedStatus();

	//Wrapper to allow access to the Pawns TileMovementComponent move directions
	bool CanMoveUp()    const;
	bool CanMoveDown()  const;
	bool CanMoveLeft()  const;
	bool CanMoveRight() const;

	bool CanMoveInDirection(EMoveDirection MoveDirection) const;

	ATile* GetCurrentTile() const;

private:
	UGameplayStatics* GameplayStatics;
	ALinkedPlayerController* PlayerController;

	//Components
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(EditAnywhere)
	UTileMovementComponent* TileMovementComponent;
	UPROPERTY(EditAnywhere)
	UDirectionComponent* DirectionComponent;
	UPROPERTY(EditAnywhere)
	UInteractComponent* InteractComponent;
	UPROPERTY(EditAnywhere)
	ULinkComponent* LinkComponent;
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* NiagaraComponent;	

	//Update the link - using the TileMovementComponent and LinkComponent
	void UpdateLink();

	//Function to be called in the editor to move the pawn to its starting tile
	UFUNCTION(CallInEditor, Category = "Setup")
	void SetActorStartLocation();

	//Returns the tile that the pawn is currently facing towards
	ATile* GetTileInDirection(EFaceDirection& OutDirection);

	//Get the MoveDirection to push a block using the FaceDirection
	EMoveDirection DeterminePushDirection(EFaceDirection FaceDirection);
};
