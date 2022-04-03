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

	Pivot->SetSimulatePhysics(true);
	Pivot->SetGenerateOverlapEvents(false);

	VehicleMesh->SetNotifyRigidBodyCollision(true);
	VehicleMesh->SetGenerateOverlapEvents(true);
	VehicleMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	VehicleMesh->SetAllMassScale(Mass);
}

// Called every frame
void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_TimeDelta = DeltaTime;

	UpdateAcceleration();
	UpdateFrictionBraking();
	UpdateMomentumAngle();
	UpdateSteering();
	UpdateRumble();
	UpdateDownforce();
	UpdateGripLevel();

	Pivot->AddWorldOffset(m_CurrentMomentumDirection * Speed * m_TimeDelta * (TopSpeed * 100));
	Pivot->AddWorldRotation(FRotator(0, m_Rotation * SteeringAmount * m_TimeDelta, 0));

	// Debug Log Variables
	/*
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, "Grip: " + FString::SanitizeFloat(GripLevel));
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, "Ang Grip: " + FString::SanitizeFloat(m_AngularGripLevel));
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, "Spd Grip: " + FString::SanitizeFloat(m_SpeedGripLevel));
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, "Brk Grip: " + FString::SanitizeFloat(m_BrakeGripLevel));
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, "Speed: " + FString::SanitizeFloat(m_Speed));
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, "Downforce: " + FString::SanitizeFloat(m_CurrentDownforce));
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, "Understeer: " + FString::SanitizeFloat(m_UndersteerAmount));
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, "Rot: " + FString::SanitizeFloat(m_Rotation));
	*/
}

void ACar::UpdateAcceleration()
{
	float accel = Acceleration  * m_Throttle * m_TimeDelta;
	float brake = BrakeStrength * m_Brake    * m_TimeDelta;

	float val = Speed + accel + -brake;

	Speed = FMath::Clamp(val, -0.1f,  1.0f);
}

void ACar::UpdateFrictionBraking()
{
	if(Speed > 0)
		Speed = FMath::Clamp(Speed - (Friction * m_TimeDelta), -0.1f, 1.0f);
}

void ACar::UpdateMomentumAngle()
{
	m_CurrentMomentumDirection = FMath::Lerp(m_CurrentMomentumDirection, Pivot->GetForwardVector(), GripLevel * 10 * m_TimeDelta);
}

void ACar::UpdateSteering()
{
	float inputRotation = UKismetMathLibrary::Lerp(m_Rotation, m_SteeringInput * FMath::Clamp(UKismetMathLibrary::NormalizeToRange(Speed, 0, 0.1f), -0.1f, 1.0f), SteeringSmoothness * m_TimeDelta);
	m_UndersteerAmount = FMath::Clamp(UKismetMathLibrary::NormalizeToRange(m_CurrentDownforce * GripLevel * UndersteerAmount, 0, 1.0f), 0.0f, 1.0f);
	m_Rotation = inputRotation * (1 + -m_UndersteerAmount);
}

void ACar::UpdateRumble()
{
	float cornerRumble = (abs(m_Rotation) * Speed) * Speed > 0.1f * RumbleSnensitivity ? 0.75f : 0;
	float gripRumble = GripLevel < 0.6f * RumbleSnensitivity ? GripLevel : 0;
	float brakeRumble = m_Brake * Speed * RumbleSnensitivity;
	float accelerateRumble = (1 + -Speed) * m_Throttle * 0.01f;
	Rumble = std::max(std::max(cornerRumble * 2, accelerateRumble), std::max(gripRumble * 2, brakeRumble));
}

void ACar::UpdateDownforce()
{
	m_CurrentDownforce = Speed > 0.1f ? Speed : 0;
}

void ACar::UpdateGripLevel()
{
	m_AngularGripLevel = 1 + -(m_CurrentDownforce * abs(m_Rotation) * (1 + -Handling / 2));
	m_SpeedGripLevel = 1 + -(Speed * (1 + -Handling / 2));
	m_BrakeGripLevel = FMath::Clamp(1 + -(m_Brake * m_CurrentDownforce * ((1 + -Handling) * BrakeTractionLossMultiplier)), 0.0f, 1.0f);
	GripLevel = std::min(m_AngularGripLevel, std::min(m_SpeedGripLevel, m_BrakeGripLevel));

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

