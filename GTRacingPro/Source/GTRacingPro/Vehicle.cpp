// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Vehicle.h"
#include "Kismet/KismetMathLibrary.h"

AVehicle::AVehicle()
{
	PrimaryActorTick.bCanEverTick = true;

	Pivot = CreateDefaultSubobject<UBoxComponent>(TEXT("Pivot"));
	VehicleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarMesh"));
	VehicleMesh->SetupAttachment(Pivot);
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Springarm"));
	SpringArm->SetupAttachment(VehicleMesh);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	VehicleMesh->SetWorldRotation(FRotator(0, 0, -90));
}

// Called when the game starts or when spawned
void AVehicle::BeginPlay()
{
	Super::BeginPlay();
	EnableInput(GetWorld()->GetFirstPlayerController());
	Pivot->SetSimulatePhysics(true);

	VehicleMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	VehicleMesh->SetAllMassScale(Mass);
}

// Called every frame
void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_TimeDelta = DeltaTime;

	UpdateAcceleration();
	UpdateFrictionBraking();

	Pivot->AddWorldOffset(Pivot->GetForwardVector() * m_Speed * m_TimeDelta * (TopSpeed * 100));
}

void AVehicle::UpdateAcceleration()
{
	float accel = Acceleration  * m_Throttle * m_TimeDelta;
	float brake = BrakeStrength * m_Brake    * m_TimeDelta;

	float val = m_Speed + accel + -brake;

	m_Speed = FMath::Clamp(val, 0.0f,  1.0f);
}

void AVehicle::UpdateFrictionBraking()
{
	m_Speed = FMath::Clamp(m_Speed - (Friction * m_TimeDelta), 0.0f, 1.0f);
}

void AVehicle::Throttle(float amount)
{
	m_Brake = abs(amount < 0 ? amount : 0);
	m_Throttle = amount > 0 ? amount : 0;
}

void AVehicle::Steer(float direction)
{
	m_SteeringInput = direction;
}

