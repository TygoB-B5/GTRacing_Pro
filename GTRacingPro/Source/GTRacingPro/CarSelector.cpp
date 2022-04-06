// Fill out your copyright notice in the Description page of Project Settings.


#include "CarSelector.h"
#include "Engine/World.h"

// Sets default values
ACarSelector::ACarSelector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
}

// Called when the game starts or when spawned
void ACarSelector::BeginPlay()
{
	if (Cars.Num() == 0)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("No Car Objects Referenced in ACarSelector"));

	SetShownCar(0);
	
	Super::BeginPlay();
}

void ACarSelector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACarSelector::SetShownCar(uint32_t index)
{
	if(m_ActiveCar)
		m_ActiveCar->K2_DestroyActor();

	m_ActiveCar = GetWorld()->SpawnActor<ACar>(Cars[index], m_SpawnPoint->GetComponentLocation(), m_SpawnPoint->GetComponentRotation());
}

void ACarSelector::SwitchCar(EDirection direction)
{
	switch (direction)
	{
	case EDirection::Left:  m_ActiveCarIndex = m_ActiveCarIndex > 0 ? m_ActiveCarIndex - 1 : Cars.Num() - 1;                break;
	case EDirection::Right: m_ActiveCarIndex = m_ActiveCarIndex < Cars.Num() - 1 ? m_ActiveCarIndex + 1 : m_ActiveCarIndex; break;
	}

	SetShownCar(m_ActiveCarIndex);
}

