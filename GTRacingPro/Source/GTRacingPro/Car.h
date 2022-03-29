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


	void UpdateAcceleration();
	void UpdateFrictionBraking();
	void UpdateMomentumAngle();
	void UpdateSteering();

private:

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

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
	float TopSpeed = 75.0f;

	UPROPERTY(EditAnywhere, Category = "Vehicle")
	float Handling = 0.33f;

	UPROPERTY(EditAnywhere, Category = "Vehicle")
	float BrakeStrength = 0.33;

	UPROPERTY(EditAnywhere, Category = "Vehicle")
	float Mass = 400;

	UPROPERTY(EditAnywhere, Category = "Vehicle")
	float Friction = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Vehicle")
	float SteeringAmount = 65;

	UPROPERTY(EditAnywhere, Category = "Vehicle")
	float SteeringSmoothness = 3;

	UPROPERTY(EditAnywhere, Category = "Vehicle")
	float RumbleSnensitivity = 0.05f;


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


	// Car Movement Variables 
	float m_Speed = 0;
	float m_Rotation = 0;
	FVector m_Momentum = FVector(0, 0, 0);

	// Player Input Variables
	float m_SteeringInput = 0;
	float m_Throttle = 0;
	float m_Brake = 0;
	bool m_bIsAirborne = false;

	float m_TimeDelta = 0;
};
