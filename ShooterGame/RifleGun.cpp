// Fill out your copyright notice in the Description page of Project Settings.


#include "RifleGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"


// Sets default values
ARifleGun::ARifleGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

void ARifleGun::PullTrigger()
{
	bool CanShoot = Ammo > 0;

	if (CanShoot)
	{

		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

		FHitResult Hit;
		FVector ShotDirection;
		Ammo--;
		bool bSuccess = GunTrace(Hit, ShotDirection);
		if (bSuccess)
		{
			//DrawDebugPoint(GetWorld(), Hit.Location, 30, FColor::Green, true);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);

			AActor* HitActor = Hit.GetActor();
			
			
			if (HitActor != nullptr)
			{
				//Headshot System
				if (Hit.BoneName == "head")
				{
					FPointDamageEvent DamageEvent(HeadShotDamage, Hit, ShotDirection, nullptr);
					AController* OwnerController = GetOwnerController();
					HitActor->TakeDamage(HeadShotDamage, DamageEvent, OwnerController, this);
				}
				else
				{
					FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
					AController* OwnerController = GetOwnerController();
					HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
				}

				//spawn blood if aCharacter
				if (HitActor->GetClass()->IsChildOf(ACharacter::StaticClass()))
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Blood, Hit.Location, ShotDirection.Rotation());
				}
			}

		}
	}	
}

void ARifleGun::GetAmmo()
{
	MaxAmmo += MaxMagazine;
}

// Called when the game starts or when spawned
void ARifleGun::BeginPlay()
{
	Super::BeginPlay();

	Ammo = MaxMagazine;
}

// Called every frame
void ARifleGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARifleGun::Reload()
{
	if (Ammo <= (MaxMagazine - 1) && MaxAmmo > 0)
	{
		MaxAmmo = MaxAmmo - (MaxMagazine - Ammo);
		Ammo = MaxMagazine;
	}
}

bool ARifleGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) 
		return false;

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* ARifleGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) 
		return nullptr;

	return OwnerPawn->GetController();
	
}

