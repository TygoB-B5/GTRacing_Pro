// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Car.h"
#include "GameFramework/Actor.h"
#include "CarSelector.generated.h"

UENUM()
enum Direction
{
	Left     UMETA(DisplayName = "Left"),
	Right    UMETA(DisplayName = "Right"),
};

UCLASS()
class GTRACINGPRO_API ACarSelector : public AActor
{
	GENERATED_BODY()
	
public:	
	ACarSelector();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	void SetShownCar(uint32_t index);

public:
	// Blueprint Variables
	UPROPERTY(EditAnywhere, Category = "Car")
	TArray<TSubclassOf<ACar>> Cars;

	// Blueprint callable Function
	UFUNCTION(BlueprintCallable, Category = "Car")
	ACar* GetActiveCar() const { return m_ActiveCar; }

	UFUNCTION(BlueprintCallable, Category = "Input")
	void SwitchCar(Direction direction);

	ACar* m_ActiveCar;
	USceneComponent* m_SpawnPoint;
	int m_ActiveCarIndex = 0;
};
