// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Timer.generated.h"

UCLASS()
class GTRACINGPRO_API ATimer : public AActor
{
	GENERATED_BODY()
	
public:	
	ATimer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnHitStart(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHitFinish(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Timer")
	UBoxComponent* Start;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Timer")
	UBoxComponent* Finish;

	UFUNCTION(BlueprintImplementableEvent, Category = "Timer")
	void OnFinish(float time);

	UFUNCTION(BlueprintImplementableEvent, Category = "Timer")
	void OnStart();

	// Getters and Setters
	UFUNCTION(BlueprintCallable, Category = "Timer")
	float GetTimerTime() const { return m_Time; }

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void SetCarActor(AActor* actor) { m_RecordableCar = actor; }

	AActor* m_RecordableCar;
	bool m_bTimerStarted = false;
	bool m_bIsGoingBackwards = false;
	float m_Time;
};
