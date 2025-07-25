// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Components/ActorComponent.h"
#include "FighterFlight.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIRJET_API UFighterFlight : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFighterFlight();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Flight")
	AActor* FlightBody;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
