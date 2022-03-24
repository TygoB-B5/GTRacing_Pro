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
	// Sets default values for this actor's properties
	AVehicle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
	float HP;

	UPROPERTY(EditAnywhere, Category = "Characteristics")
	float Handling;

	UPROPERTY(EditAnywhere, Category = "Characteristics")
	float BrakeStrength;

	UPROPERTY(EditAnywhere, Category = "Characteristics")
	float Mass;

	UPROPERTY(EditAnywhere, Category = "Characteristics")
	float SteerStrength;

	UPROPERTY(EditAnywhere, Category = "Characteristics")
	float SpringBackStrength;

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void Throttle(float amount);

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void Steer(float direction);

	FVector Velocity = FVector(0, 0, 0);
	float ZRotation = 0;

	float TimeDelta = 0;
};
