// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DefaultEnemy.generated.h"

UCLASS()
class CSE381HW1_API ADefaultEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADefaultEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float _damage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	bool hasTakenDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	bool isDead;
	

};
