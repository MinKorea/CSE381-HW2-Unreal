// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerProjectile.h"
#include "MainCharacter.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"

FTimerHandle ProjectileTimerHandle;

// Sets default values
APlayerProjectile::APlayerProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    if (!RootComponent)
    {
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
    }

    if (!CollisionComponent)
    {
        // Use a sphere as a simple collision representation.
        CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
        // Set the sphere's collision profile name to "Projectile".
        CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));

        // Event called when component hits something.
        CollisionComponent->OnComponentHit.AddDynamic(this, &APlayerProjectile::OnHit);
        
        // Set the sphere's collision radius.
        CollisionComponent->InitSphereRadius(15.0f);
        // Set the root component to be the collision component.
        RootComponent = CollisionComponent;


    }

    static ConstructorHelpers::FObjectFinder<USoundCue> SoundCueObject(TEXT("/Script/Engine.SoundCue'/Game/Audios/Sound_BallHit_Cue.Sound_BallHit_Cue'"));
    
    if (SoundCueObject.Succeeded())
    {
        SoundCue = SoundCueObject.Object;

        // Setup the Audio Component
        SoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound Effect"));
        SoundEffect->SetupAttachment(RootComponent);  // Attach to root component
        SoundEffect->SetSound(SoundCue);  // Set the sound cue
    }

    if (!ProjectileMovementComponent)
    {
        // Use this component to drive this projectile's movement.
        ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
        ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
        ProjectileMovementComponent->InitialSpeed = 3000.0f;
        ProjectileMovementComponent->MaxSpeed = 3000.0f;
        ProjectileMovementComponent->bRotationFollowsVelocity = true;
        ProjectileMovementComponent->bShouldBounce = true;
        ProjectileMovementComponent->Bounciness = 0.3f;
        ProjectileMovementComponent->ProjectileGravityScale = 1.0f;
    }

    if (!ProjectileMeshComponent)
    {
        ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
        static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/Meshes/Sphere.Sphere'"));
        if (Mesh.Succeeded())
        {
            ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
        }

        static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/Materials/SphereMaterial.SphereMaterial'"));
        if (Material.Succeeded())
        {
            ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
        }
        ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
        ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
        ProjectileMeshComponent->SetupAttachment(RootComponent);
    }

    
    // Delete the projectile after 3 seconds.
    // InitialLifeSpan = 3.0f;

}

// Called when the game starts or when spawned
void APlayerProjectile::BeginPlay()
{
    Super::BeginPlay();

    // GetWorldTimerManager().SetTimer(ProjectileTimerHandle, this, &APlayerProjectile::DestroyOwnedBall, 5.0f, false, 5.0f);
}

// Called every frame
void APlayerProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (owned)
    {
        if (ProjectileMovementComponent->Velocity.Length() < 1.0f)
        {
            owned = false;
            Destroy();
        }
    }

}

// Function that initializes the projectile's velocity in the shoot direction.
void APlayerProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

// Function that is called when the projectile hits something.
void APlayerProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    // Play the sound
    SoundEffect->Play();

    if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
    {
        // GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("OnHit Called"));
        OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
    }
}

void APlayerProjectile::DestroyOwnedBall()
{
    if (owned)
    {
        Destroy();
    }
}