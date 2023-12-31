// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "PlayerProjectile.generated.h"

UCLASS()
class CSE381HW1_API APlayerProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerProjectile();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// True if the projectile is owend by a character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	bool owned = false;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sphere collision component.
	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Projectile)
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	
	USphereComponent* CollisionComponent;

	// Projectile movement component.
	UPROPERTY(VisibleAnywhere,BlueprintreadWrite, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	// Function that initializes the projectile's velocity in the shoot direction.
	UFUNCTION(BlueprintCallable)
	void FireInDirection(const FVector& ShootDirection);

	// Projectile mesh
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* ProjectileMeshComponent;

	// Projectile material
	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
	UMaterialInstanceDynamic* ProjectileMaterialInstance;

	// Function that is called when the projectile hits something.
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	UAudioComponent* SoundEffect;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	USoundCue* SoundCue;
	

	void DestroyOwnedBall();
};
