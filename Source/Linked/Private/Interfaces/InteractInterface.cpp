// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/InteractInterface.h"

// Add default functionality here for any IInteractInterface functions that are not pure virtual.

void IInteractInterface::InteractWithObject()
{
	UE_LOG(LogTemp, Warning, TEXT("Interact method called from IInteractInterface!"));
}
