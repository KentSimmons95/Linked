// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"

class ALinkedPlayerPawn;

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

	void Interact();

private:
	//Length of the line trace is the size of the game tile
	float LineTraceLength = 100.0f;

	const float Right = 90.0f;

	FVector CurrentLocation;
	FRotator CurrentRotation;

	//Actor Parent is ALinkedPlayerPawn - only Player Pawns will have the ability to line trace
	ALinkedPlayerPawn* ActorOwner;
	AActor* TargetHit;

	//Line Trace that shoots in the direction the actor is currently facing
	void LineTrace();
	//Calculate the end location for the linetrace when fired
	FVector CalculateEndLocation();

	AActor* ActorHit();
};
