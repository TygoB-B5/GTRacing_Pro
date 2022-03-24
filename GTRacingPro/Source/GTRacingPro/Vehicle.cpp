// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Vehicle.h"


// Sets default values
AVehicle::AVehicle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Pivot = CreateDefaultSubobject<UBoxComponent>(TEXT("Pivot"));
	VehicleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarMesh"));
	VehicleMesh->SetupAttachment(Pivot);
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Springarm"));
	SpringArm->SetupAttachment(VehicleMesh);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void AVehicle::BeginPlay()
{
	Super::BeginPlay();
	EnableInput(GetWorld()->GetFirstPlayerController());
	Pivot->SetSimulatePhysics(true);

	VehicleMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
}

// Called every frame
void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeDelta = DeltaTime;

	Pivot->AddWorldOffset(Pivot->GetForwardVector() * Velocity.Y * DeltaTime);
	Pivot->AddWorldRotation(FRotator(0, ZRotation, 0));

	ZRotation = FMath::Lerp(ZRotation, 0.0f, TimeDelta * SpringBackStrength);
	Velocity.Y -= TimeDelta * Velocity.Y < 0 ? -Mass : Mass;

}

void AVehicle::Throttle(float amount)
{
	if (amount > 0)
	{	
		Velocity.Y += HP * TimeDelta;
	}
	if (amount < 0)
	{
		if (Velocity.Y < -200)
			return;

		Velocity.Y -= BrakeStrength * TimeDelta;
	}
}

void AVehicle::Steer(float direction)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::SanitizeFloat(ZRotation));
	ZRotation += direction * SteerStrength * TimeDelta;
}

