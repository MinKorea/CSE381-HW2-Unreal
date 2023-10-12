// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "PlayerProjectile.h"


APlayerProjectile* Ball;

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a first person camera component.
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr);

	// Attach the camera component to our capsule component.
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

	// Position the camera slightly above the eyes.
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

	// Enable the pawn to control camera rotation.
	FPSCameraComponent->bUsePawnControlRotation = true;

	// Create a first person mesh component for the owning player.
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FPSMesh != nullptr);

	// Only the owning player sees this mesh.
	FPSMesh->SetOnlyOwnerSee(true);

	// Attach the FPS mesh to the FPS camera.
	FPSMesh->SetupAttachment(FPSCameraComponent);

	// Event called when component hits something.
	HitCapsule = GetCapsuleComponent();
	HitCapsule->BodyInstance.SetCollisionProfileName(TEXT("MainCharacter"));
	HitCapsule->OnComponentHit.AddDynamic(this, &AMainCharacter::OnHit);
	/*GetCapsuleComponent()->BodyInstance.SetCollisionProfileName(TEXT("MainCharacter"));
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMainCharacter::OnHit);*/

	// Disable some environmental shadows to preserve the illusion of having a single mesh.
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	// The owning player doesn't see the regular (third-person) body mesh.
	GetMesh()->SetOwnerNoSee(true);

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHaveBall)
	{
		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		MuzzleOffset.Set(150.0f, 50.0f, -50.0f);
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		Ball->SetActorLocation(MuzzleLocation);
		
	}

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	// Set up "movement" bindings.
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	// Set up "look" bindings.
	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::AddControllerPitchInput);

	// Set up "action" bindings.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainCharacter::StopJump);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::Fire);
}

void AMainCharacter::MoveForward(float Value)
{
	// Find out which way is "forward" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AMainCharacter::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AMainCharacter::StartJump()
{
	bPressedJump = true;
}

void AMainCharacter::StopJump()
{
	bPressedJump = false;
}

void AMainCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Main character hit Something"));

	if (APlayerProjectile* HitActor = Cast<APlayerProjectile>(OtherActor))
	{
		// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Main character hit the projectile"));
		if (HitActor->owned)
		{

		}	
		else if (!bHaveBall)
		{
			bHaveBall = true;

			UWorld* World = GetWorld();
			if (World)
			{
				// Get the camera transform.
				FVector CameraLocation;
				FRotator CameraRotation;
				GetActorEyesViewPoint(CameraLocation, CameraRotation);

				// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
				MuzzleOffset.Set(100.0f, 50.0f, -50.0f);

				// Transform MuzzleOffset from camera space to world space.
				FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

				// Skew the aim to be slightly upwards.
				FRotator MuzzleRotation = CameraRotation;
				MuzzleRotation.Pitch += 10.0f;

				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				// SpawnParams.Instigator = GetInstigator(); 
				// Spawn the projectile at the muzzle.
				Ball = World->SpawnActor<APlayerProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
				Ball->ProjectileMovementComponent->InitialSpeed = 0.0f;
				Ball->ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
				Ball->ProjectileMovementComponent->Velocity = Ball->ProjectileMovementComponent->Velocity * 0;
				HitActor->Destroy();
			}
		}	
	}
}

void AMainCharacter::Fire()
{
	// Attempt to fire a projectile.
	if (ProjectileClass && bHaveBall)
	{
		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
		MuzzleOffset.Set(100.0f, 50.0f, -50.0f);	

		// Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		// Skew the aim to be slightly upwards.
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 10.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// Spawn the projectile at the muzzle.
			APlayerProjectile* Projectile = World->SpawnActor<APlayerProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Fire!"));

				// Set the projectile's initial trajectory.
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
				Projectile->owned = true;
				bHaveBall = false; // added code
				Ball->Destroy();   // added code
			}
		}
	}
}