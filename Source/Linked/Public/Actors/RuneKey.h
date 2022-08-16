// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "RuneKey.generated.h"

UCLASS()
class LINKED_API ARuneKey : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARuneKey();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere)
	USceneComponent* PickupRoot;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditAnywhere, Category = "Actor Location")
	UBoxComponent* OverlapBox;

	//Sets the RuneKey location to the specified tile actors location
	UPROPERTY(EditAnywhere)
	class ATile* StartLocation = nullptr;

	//Sets the exit that this key will unlock for
	UPROPERTY(EditAnywhere, Category = "Key Properties")
	class ARuneExit* RuneExit;
	
	//Moves the RuneKey Actor to the start location 
	UFUNCTION(CallInEditor, Category = "Actor Location")
	void MoveToTileLocation();

	UFUNCTION()
	void OnPlayerEnterOverlapBox(UPrimitiveComponent* OverlappedActor, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		
};
