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
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Creates a link with an Actor
	//void CreateLinkWithActor(AActor* ActorToLink);
	void CreateLinkWithActor();
	
	AActor* GetCurrentLinkedActor();
	bool IsCurrentlyLinked();

	//Called by the Actor Owner once the move have finished
	void UpdateLinkStatus();

private:

	AActor* ActorOwner = nullptr;
	//The actor that is linked to this actor
	UPROPERTY(EditAnywhere, Category = "Target")
	AActor* ActorToLink = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Link Status")
	bool bIsCurrentlyLinked = false;
	UPROPERTY(VisibleAnywhere, Category = "Link Status")
	bool bIsSightBlocked = false;

	bool HasLineOfSight();
};
