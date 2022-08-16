// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "World/LinkedGameMode.h"
#include "RuneExit.generated.h"

class ARuneKey;

/* The RuneExit clas is used for creating an end of level objective for the player
*  It has an associated RuneKey actor that must be collected before it is enabled
*/

UCLASS()
class LINKED_API ARuneExit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARuneExit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	/*Called by the Key that references this RuneExit
	* Enables the Exit OverlapBox to trigger events
	*/
	void KeyCollected();

	bool IsPawnStandingOnExit() const;
private:

	//Root component of the exit
	UPROPERTY(EditAnywhere)
	USceneComponent* PickupRoot;

	//Trigger volume to detect overlapping actors
	UPROPERTY(EditAnywhere)
	UBoxComponent* OverlapBox;

	//Visual effect of the actor
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* NiagaraComponent;

	//Set the RuneExits location to the tile location
	UPROPERTY(EditAnywhere)
	class ATile* TileStartLocation = nullptr;

	//The game mode
	ALinkedGameMode* LinkedGameMode = nullptr;
	bool bKeyCollected = false;
	bool bIsPawnStandingOnExit = false;

	//Test if overlapping actor is a LinkedPlayerPawn
	bool IsAPlayerPawn(AActor* OtherActor);

	UFUNCTION()
	void OnPlayerEnterOverlapBox(UPrimitiveComponent* OverlappedActor, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerLeaveOverlapBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
