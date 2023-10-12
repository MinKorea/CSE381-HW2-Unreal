// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Components/BoxComponent.h"
#include "PlayerProjectile.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "DefaultEnemy.h"

FTimerHandle EnemyMemberTimerHandle;

// Sets default values
AEnemySpawner::AEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	SpawnVolume->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(EnemyMemberTimerHandle, this, &AEnemySpawner::SpawnActor, 1.0f, true, 2.0f);

}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::SpawnActor()
{
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();

	FVector coordinates[30];

	for (int i = 0; i < 30; i++)
	{
		int randX = FMath::RandRange(-9, 9) * 31;
		int randY = FMath::RandRange(-9, 9) * 31;
		int randZ = FMath::RandRange(-8, 8) * 31;

		coordinates[i].X = randX;
		coordinates[i].Y = randY;
		coordinates[i].Z = randZ;

		for (int j = 0; j < i; j++)
		{
			if (coordinates[i].X == coordinates[j].X && coordinates[i].Y == coordinates[j].Y && coordinates[i].Z == coordinates[j].Z)
			{
				i--;
			}
		}

	}

	for (int i = 0; i < 30; i++)
	{
		SpawnLocation.X += coordinates[i].X;
		SpawnLocation.Y += coordinates[i].Y;
		SpawnLocation.Z += coordinates[i].Z;

		SpawnLocation.Set(SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);

		ADefaultEnemy* Enemy;

		Enemy = GetWorld()->SpawnActor<ADefaultEnemy>(SpawnLocation, SpawnRotation);
		SpawnLocation = GetActorLocation();
	}
}