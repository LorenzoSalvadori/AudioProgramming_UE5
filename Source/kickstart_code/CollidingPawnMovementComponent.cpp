// Fill out your copyright notice in the Description page of Project Settings.


#include "CollidingPawnMovementComponent.h"

void UCollidingPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) 
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) 
	{
		return;
	}

	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * 150.0f; // 150.0f is a hard-coded MAX_SPEED
	if (!DesiredMovementThisFrame.IsNearlyZero()) 
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

		// if we bump into something
		if (Hit.IsValidBlockingHit()) 
		{
			SlideAlongSurface(DesiredMovementThisFrame, 1.0f-Hit.Time, Hit.Normal, Hit);
		}
	}
}