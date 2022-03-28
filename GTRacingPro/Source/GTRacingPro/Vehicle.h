// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Vehicle.generated.h"

UCLASS()
class GTRACINGPRO_API AVehicle : public AActor
{
	GENERATED_BODY()
	
public:	
	AVehicle();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


	void UpdateAcceleration();
	void UpdateFrictionBraking();

public:
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Pivot;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* VehicleMesh;

	UPROPERTY(EditAnywhere, Category = "Characteristics")
	float Acceleration;

	UPROPERTY(EditAnywhere, Category = "Characteristics")
	float TopSpeed;

	UPROPERTY(EditAnywhere, Category = "Characteristics")
	float Handling;

	UPROPERTY(EditAnywhere, Category = "Characteristics")
	float BrakeStrength;

	UPROPERTY(EditAnywhere, Category = "Characteristics")
	float Mass;

	UPROPERTY(EditAnywhere, Category = "Characteristics")
	float Friction;

	UPROPERTY(EditAnywhere, Category = "Characteristics")
	float SteerStrength;

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void Throttle(float amount);

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void Steer(float direction);

	float m_Speed = 0;
	float m_SteeringInput = 0;
	float m_Throttle = 0;
	float m_Brake = 0;

	float m_TimeDelta = 0;
};
