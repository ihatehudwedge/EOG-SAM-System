// Fill out your copyright notice in the Description page of Project Settings.


#include "FighterFlight.h"

// Sets default values for this component's properties
UFighterFlight::UFighterFlight()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFighterFlight::BeginPlay()
{
	Super::BeginPlay();

	// ...
	FlightBody = GetOwner();
	if (FlightBody)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Aircraft value catch!"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No..."));
	}
}


// Called every frame
void UFighterFlight::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	float speed = 58.0f;
	
	FVector ForwardVector = FlightBody->GetActorForwardVector();
	ForwardVector *= speed;

	FlightBody->AddActorWorldOffset(ForwardVector);
}

