// Fill out your copyright notice in the Description page of Project Settings.


#include "Timer.h"

// Sets default values
ATimer::ATimer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Start = CreateDefaultSubobject<UBoxComponent>(TEXT("Start"));
	Finish = CreateDefaultSubobject<UBoxComponent>(TEXT("Finish"));

	Start->SetGenerateOverlapEvents(true);
	Finish->SetGenerateOverlapEvents(true);

	Start->SetNotifyRigidBodyCollision(true);
	Finish->SetNotifyRigidBodyCollision(true);

	Start->SetCollisionProfileName(TEXT("OverlapAll"));
	Finish->SetCollisionProfileName(TEXT("OverlapAll"));

	Start->OnComponentBeginOverlap.AddDynamic(this, &ATimer::OnHitStart);
	Finish->OnComponentBeginOverlap.AddDynamic(this, &ATimer::OnHitFinish);
}

// Called when the game starts or when spawned
void ATimer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATimer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_bTimerStarted)
		m_Time += 1 * DeltaTime;
}

void ATimer::OnHitStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == m_RecordableCar)
	{
		m_bIsGoingBackwards = m_bTimerStarted && !m_bIsGoingBackwards;

		if (!m_bTimerStarted)
		{
			OnStart();
			m_bTimerStarted = true;
		}
	}
}

void ATimer::OnHitFinish(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == m_RecordableCar && m_bTimerStarted)
	{
		if (!m_bIsGoingBackwards)
		{
			OnFinish(m_Time);
			m_bTimerStarted = false;
			m_Time = 0;
		}
	}
}