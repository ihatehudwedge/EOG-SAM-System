// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImageUtils.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Engine/Engine.h"
#include "Components/ActorComponent.h"
#include "MissileSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIRJET_API UMissileSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMissileSystem();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Missile")
	AActor* MissileBody;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Physics")
	void Launch(float Speed);

	UFUNCTION(BlueprintCallable, Category = "Physics")
	void RotateMissile(FString Command, float Gap);

	UFUNCTION(BlueprintCallable, Category = "Utils")
	void SaveRenderTargetToDisk(UTextureRenderTarget2D* RenderTarget, FString FilePath);
};
