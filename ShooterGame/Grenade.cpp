// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/EngineTypes.h"
#include "ShootherChar.h"


// Sets default values
AGrenade::AGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetSphereRadius(12.f);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent = Collision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grenade Mesh"));
	Mesh->SetupAttachment(Collision);

	SphereDamage = CreateDefaultSubobject<USphereComponent>(TEXT("Damage Sphere"));
	SphereDamage->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
}

void AGrenade::Explode()
{
	FVector ExplosionLoc = Mesh->GetComponentLocation();
	SphereDamage->SetWorldLocation(ExplosionLoc);

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, ExplosionLoc);
	}
	if (Explosion)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, ExplosionLoc);
	}


	//Damage system
	TArray<AActor*> OverlappingActors;
	SphereDamage->GetOverlappingActors(OverlappingActors, TSubclassOf<AShootherChar>());

	for (AActor* Actor : OverlappingActors)
	{
		if (AShootherChar* Char = Cast<AShootherChar>(Actor))
		{
			Char->TakeDamage(Damage, FDamageEvent(), nullptr, this);
		}
	}

	Destroy();
}

void AGrenade::Throw(FVector ForwardVector)
{
	ForwardVector *= ThrowForce;

	Mesh->SetSimulatePhysics(true);
	Mesh->SetEnableGravity(true);
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Mesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	Mesh->AddImpulse(ForwardVector);
	GetWorldTimerManager().SetTimer(Timer, this, &AGrenade::Explode,1.5f, false);

	//Tracking grenade amount in shoothercpp
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
