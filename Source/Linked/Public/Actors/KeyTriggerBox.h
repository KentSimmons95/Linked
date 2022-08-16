// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "KeyTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class LINKED_API AKeyTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	
public:

	AKeyTriggerBox();

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	bool TriggeredByPlayer = false;

	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
};
