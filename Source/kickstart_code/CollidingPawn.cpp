// Fill out your copyright notice in the Description page of Project Settings.


#include "CollidingPawn.h"

#include<UObject/ConstructorHelpers.h>
#include<Particles/ParticleSystemComponent.h>
#include<Components/SphereComponent.h>
#include<Camera/CameraComponent.h>
#include<GameFramework/SpringArmComponent.h>

#include "CollidingPawnMovementComponent.h"

// Sets default values
ACollidingPawn::ACollidingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a sphere object and make it the root component
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent; // A RootComponent is the component that deals with the Transform of an instance of a APawn
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	// Associate a Mesh component to the sphere to make it visible
	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded()) 
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		SphereVisual->SetWorldScale3D(FVector(0.8f));
	}

	// create a particle system that can be activated/deactivated
	OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	OurParticleSystem->SetupAttachment(SphereVisual);
	OurParticleSystem->bAutoActivate = false;
	OurParticleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
	if (ParticleAsset.Succeeded()) 
	{
		OurParticleSystem->SetTemplate((ParticleAsset.Object));
	}

	// setting up a spring arm for smooth camera motion
	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.f, 0.f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	// create the camera and attach it to the spring
	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// Take control of default player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// create the movement component and attach to root
	OurMovementComponent = CreateDefaultSubobject<UCollidingPawnMovementComponent>(TEXT("CustomMovement"));
	//OurMovementComponent->UpdatedComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ACollidingPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollidingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACollidingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("ParticleToggle", IE_Pressed, this, &ACollidingPawn::ParticleToggle);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACollidingPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACollidingPawn::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACollidingPawn::Turn);
}

UPawnMovementComponent* ACollidingPawn::GetMovementComponent() const
{
	return OurMovementComponent;
}

void ACollidingPawn::MoveForward(float AxisValue) 
{
	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent)) 
	{
		OurMovementComponent->AddInputVector(GetActorForwardVector() * AxisValue);
	}
}

void ACollidingPawn::MoveRight(float AxisValue) 
{
	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent)) 
	{
		OurMovementComponent->AddInputVector(GetActorRightVector() * AxisValue);
	}
}

void ACollidingPawn::Turn(float AxisValue) 
{
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += AxisValue;
	SetActorRotation(NewRotation);
}

void ACollidingPawn::ParticleToggle() 
{
	if (OurParticleSystem && OurParticleSystem->Template) 
	{
		OurParticleSystem->ToggleActive();
	}
}