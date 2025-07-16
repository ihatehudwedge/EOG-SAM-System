// Fill out your copyright notice in the Description page of Project Settings.


#include "MissileSystem.h"

// Sets default values for this component's properties
UMissileSystem::UMissileSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMissileSystem::BeginPlay()
{
	Super::BeginPlay();

	MissileBody = GetOwner();

	if(MissileBody)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("- SAM is Activated"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("No..."));
	}
}


// Called every frame
void UMissileSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMissileSystem::Launch(float Speed)
{
	FVector Direction = MissileBody->GetActorForwardVector();

	MissileBody->AddActorWorldOffset(Direction * Speed);
}

void UMissileSystem::RotateMissile(FString Command, float Gap)
{
    FString Flag[5] = { TEXT("L"), TEXT("R") , TEXT("T") , TEXT("B") , TEXT("C") };

    //회전을 부여하지 않는다
    if (Command.Contains(Flag[4]))
        return;

    FRotator DeltaRotationYaw(0.f, Gap, 0.f);
    if (Command.Contains(Flag[0])) //Left
    {
        MissileBody->SetActorRotation(DeltaRotationYaw * -1.0f + MissileBody->GetActorRotation());
        UE_LOG(LogTemp, Log, TEXT("Rotate Right"));
    }
    else if (Command.Contains(Flag[1])) //Right
    {
        MissileBody->SetActorRotation(DeltaRotationYaw * 1.0f + MissileBody->GetActorRotation());
        UE_LOG(LogTemp, Log, TEXT("Rotate Left"));
    }

    FRotator DeltaRotationPitch(Gap, 0.f, 0.f);
    if (Command.Contains(Flag[2])) //Top
    {
        MissileBody->SetActorRotation(DeltaRotationPitch * 1.0f + MissileBody->GetActorRotation());
        UE_LOG(LogTemp, Log, TEXT("Rotate Bottom"));
    }
    else if (Command.Contains(Flag[3])) //Bottom
    {
        MissileBody->SetActorRotation(DeltaRotationPitch * -1.0f + MissileBody->GetActorRotation());
        UE_LOG(LogTemp, Log, TEXT("Rotate Top"));
    }

    GEngine->AddOnScreenDebugMessage(-1, 0.15f, FColor::Red, TEXT("SAM: Target Locked"));
}

void UMissileSystem::SaveRenderTargetToDisk(UTextureRenderTarget2D* RenderTarget, FString FilePath)
{
    if (!RenderTarget)
    {
        UE_LOG(LogTemp, Error, TEXT("RenderTarget is null!"));
        return;
    }

    // RenderTarget에서 텍스처 데이터 추출
    FTextureRenderTargetResource* RenderTargetResource = RenderTarget->GameThread_GetRenderTargetResource();
    if (!RenderTargetResource)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get RenderTargetResource!"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("RenderTargetResource acquired successfully."));

    TArray<FColor> Bitmap;
    RenderTargetResource->ReadPixels(Bitmap);

    if (Bitmap.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Bitmap is empty!"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Bitmap data read successfully."));

    // 이미지 데이터 압축
    int32 Width = RenderTarget->SizeX;
    int32 Height = RenderTarget->SizeY;
    TArray<uint8> CompressedData;

    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));
    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

    if (ImageWrapper.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("ImageWrapper is valid."));
        if (ImageWrapper->SetRaw(Bitmap.GetData(), Bitmap.GetAllocatedSize(), Width, Height, ERGBFormat::BGRA, 8))
        {
            CompressedData = ImageWrapper->GetCompressed();
            UE_LOG(LogTemp, Log, TEXT("Image compressed successfully."));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to set raw data for ImageWrapper!"));
            return;
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ImageWrapper is invalid!"));
        return;
    }

    // 파일 저장
    if (CompressedData.Num() > 0)
    {
        if (FFileHelper::SaveArrayToFile(CompressedData, *FilePath))
        {
            UE_LOG(LogTemp, Log, TEXT("File saved successfully to: %s"), *FilePath);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to save file to: %s"), *FilePath);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("CompressedData is empty!"));
    }
}