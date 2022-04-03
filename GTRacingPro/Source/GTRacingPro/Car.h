// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Car.generated.h"

UCLASS()
class GTRACINGPRO_API ACar : public AActor
{
	GENERATED_BODY()
	
public:	
	ACar();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:

	void UpdateAcceleration();
	void UpdateFrictionBraking();
	void UpdateMomentumAngle();
	void UpdateSteering();
	void UpdateRumble();
	void UpdateDownforce();
	void UpdateGripLevel();

public:
	// Blueprint Changable Properties
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Pivot;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* VehicleMesh;



	// Blueprint Car Settings
	UPROPERTY(EditAnywhere, Category = "Vehicle")
	float Acceleration = 0.25f;

	UPROPERTY(EditAnywhere, Category = "Vehicle")
	float TopSpeed = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Vehicle")
	float Handling = 0.33f;

	UPROPERTY(EditAnywhere, Category = "Vehicle")
	float BrakeStrength = 0.25f;

	UPROPERTY(EditAnywhere, Category = "Vehicle")
	float Mass = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Vehicle")
	float Friction = 0.05f;

	UPROPERTY(EditAnywhere, Category = "Vehicle")
	float SteeringAmount = 70.0f;

	UPROPERTY(EditAnywhere, Category = "Vehicle")
	float SteeringSmoothness = 3;

	UPROPERTY(EditAnywhere, Category = "Vehicle")
	float RumbleSnensitivity = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Vehicle")
	float UndersteerAmount = 0.15f;

	UPROPERTY(EditAnywhere, Category = "Vehicle")
	float BrakeTractionLossMultiplier = 2.0f;


	// Blueprint Functions
	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void Throttle(float amount);

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void Steer(float direction);


	// Blueprint Read Only
	UPROPERTY(BlueprintReadOnly, Category = "Vehicle")
	float GripLevel;

	UPROPERTY(BlueprintReadOnly, Category = "Vehicle")
	float Rumble;

	UPROPERTY(BlueprintReadOnly, Category = "Vehicle")
	float Speed = 0;

	// Car Movement Variables 
	float m_Rotation = 0;
	float m_CurrentDownforce = 0;
	float m_UndersteerAmount = 0;

	float m_AngularGripLevel = 0;
	float m_SpeedGripLevel = 0;
	float m_BrakeGripLevel = 0;

	FVector m_CurrentMomentumDirection = FVector(0, 0, 0);

	// Player Input Variables
	float m_SteeringInput = 0;
	float m_Throttle = 0;
	float m_Brake = 0;

	float m_TimeDelta = 0;
};
