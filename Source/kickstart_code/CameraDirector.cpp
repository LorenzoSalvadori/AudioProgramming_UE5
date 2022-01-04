// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraDirector.h"
#include <Kismet/GameplayStatics.h>
// Sets default values
ACameraDirector::ACameraDirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraDirector::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraDirector::Tick(float DeltaTime)
{
	const float timeBetweenCameraChanges = 2.0f;
	const float smoothBlendTime = 0.75f;
	timeToNextCameraChange -= DeltaTime;

	if (timeToNextCameraChange <= 0) 
	{
		timeToNextCameraChange += timeBetweenCameraChanges;

		APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (OurPlayerController) 
		{
			if (OurPlayerController->GetViewTarget() != CameraOne && CameraOne != nullptr) 
			{
				OurPlayerController->SetViewTarget(CameraOne);
			}

			else if (OurPlayerController->GetViewTarget() != CameraTwo && CameraTwo != nullptr)
			{
				OurPlayerController->SetViewTargetWithBlend(CameraTwo, smoothBlendTime);
			}

		}
	}

	Super::Tick(DeltaTime);

}

