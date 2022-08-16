// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TileMovementComponent.h"
#include "Components/InteractComponent.h"
#include "Components/DirectionComponent.h"
#include "Block.generated.h"

UCLASS()
class LINKED_API ABlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	bool CanMoveInDirection(EMoveDirection Direction);
	void MoveInDirection(EMoveDirection Direction);

private:

	//Component Setup
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditAnywhere)
	UTileMovementComponent* TileMovementComponent;

	//Moves the Block Actor to the start location 
	UFUNCTION(CallInEditor, Category = "Actor Location")
	void MoveToTileLocation();
};
