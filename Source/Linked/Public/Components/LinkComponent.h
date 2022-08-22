// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LinkComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LINKED_API ULinkComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULinkComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	//Creates a link with an Actor
	void CreateLinkWithActor();

	AActor* GetCurrentLinkedActor() const;

	//Returns the current linked status to this Actor
	bool IsCurrentlyLinked() const;

	//Checks if there are no obstructions between the owning actor and its target Actor
	bool HasLineOfSight();

private:
	//This components owning actor
	AActor* ActorOwner = nullptr;

	//The actor that is linked to this actor
	UPROPERTY(EditAnywhere, Category = "Target")
	AActor* LinkedActor = nullptr;
	
	AActor* HitActor = nullptr;

	//Flag to show if the link is active or not
	UPROPERTY(VisibleAnywhere, Category = "Link Status")
	bool bIsCurrentlyLinked = false;

	
};
