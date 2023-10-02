// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "Components/BoxComponent.h"
#include "PlayerProjectile.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"

FTimerHandle MemberTimerHandle;

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	SpawnVolume->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &ASpawner::SpawnActor, 10.0f, true, 2.0f);
	
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawner::SpawnActor()
{
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();	

	for (int i = 0; i < 30; i++)
	{
		int randX = FMath::RandRange(-300, 300);
		int randY = FMath::RandRange(-300, 300);
		int randZ = FMath::RandRange(-300, 300);

		SpawnLocation.X += randX;
		SpawnLocation.Y += randY;
		SpawnLocation.Z += randZ;

		SpawnLocation.Set(SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);

		GetWorld()->SpawnActor<APlayerProjectile>(SpawnLocation, SpawnRotation);
		SpawnLocation = GetActorLocation();
	}
}

