// Fill out your copyright notice in the Description page of Project Settings.

#include "Car.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

ACar::ACar()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup Root object
	Pivot = CreateDefaultSubobject<UBoxComponent>(TEXT("Pivot"));

	// Setup Vehicle Static Mesh Obeject
	VehicleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarMesh"));
	VehicleMesh->SetupAttachment(Pivot);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Springarm"));
	SpringArm->SetupAttachment(VehicleMesh);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ACar::BeginPlay()
{
	Super::BeginPlay();
	EnableInput(GetWorld()->GetFirstPlayerController());

	Pivot->SetSimulatePhysics(true);

	VehicleMesh->SetNotifyRigidBodyCollision(true);
	VehicleMesh->SetGenerateOverlapEvents(true);
	VehicleMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	VehicleMesh->SetAllMassScale(Mass);
	VehicleMesh->OnComponentHit.AddDynamic(this, &ACar::OnHit);
}

// Called every frame
void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_TimeDelta = DeltaTime;

	if(!m_bIsAirborne)
		UpdateAcceleration();

	UpdateFrictionBraking();
	UpdateMomentumAngle();
	UpdateSteering();

	Pivot->AddWorldOffset(m_Momentum * m_Speed * m_TimeDelta * (TopSpeed * 100));
	Pivot->AddWorldRotation(FRotator(0, m_Rotation * SteeringAmount * m_TimeDelta, 0));

	float cornerRumble = (abs(m_Rotation) * m_Speed) * m_Speed > 0.1f ? 0.75f : 0;
	float gripRumble = GripLevel > 0.6f ? GripLevel : 0;
	float brakeRumble = m_Brake * m_Speed;
	Rumble = std::max(cornerRumble, std::max(gripRumble, brakeRumble / 2));

	m_bIsAirborne = true;
}

void ACar::UpdateAcceleration()
{
	float accel = Acceleration  * m_Throttle * m_TimeDelta;
	float brake = BrakeStrength * m_Brake    * m_TimeDelta;

	float val = m_Speed + accel + -brake;

	m_Speed = FMath::Clamp(val, -0.1f,  1.0f);
}

void ACar::UpdateFrictionBraking()
{
	if(m_Speed > 0)
		m_Speed = FMath::Clamp(m_Speed - (Friction * m_TimeDelta), -0.1f, 1.0f);
}

void ACar::UpdateMomentumAngle()
{
	GripLevel = FMath::Clamp((-m_Speed + 1) * abs(m_Rotation) + (-m_Brake * 0.5f), 0.0f, 1.0f);
	m_Momentum = FMath::Lerp(m_Momentum, Pivot->GetForwardVector(), (GripLevel + Handling) * 10 * m_TimeDelta);
}

void ACar::UpdateSteering()
{
	m_Rotation = UKismetMathLibrary::Lerp(m_Rotation, m_SteeringInput * FMath::Clamp(UKismetMathLibrary::NormalizeToRange(m_Speed, 0, 0.2f), -0.1f, 1.0f), SteeringSmoothness * m_TimeDelta);
}

void ACar::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	m_bIsAirborne = false;
}

void ACar::Throttle(float amount)
{
	m_Brake = abs(amount < 0 ? amount : 0);
	m_Throttle = amount > 0 ? amount : 0;
}

void ACar::Steer(float direction)
{
	m_SteeringInput = direction;
}

