// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Actors/Tile.h"
#include "Components/TileMovementComponent.h"
#include "Components/DirectionComponent.h"

#include "GameplayTagContainer.h"

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
	EFaceDirection GetCurrentFaceDirection();

	//Calls the TileMovementComponent that is attached to the Pawn to move tile
	void Move(EMoveDirection Direction);
	void Turn(EFaceDirection Direction);

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

	//REMOVE?
	FVector MovementDirection;
	UPROPERTY()
	float MovementSpeed = 500.f;

};
