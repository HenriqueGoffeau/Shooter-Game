// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RifleGun.generated.h"


UCLASS()
class SHOOTERGAME_API ARifleGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARifleGun();

	void PullTrigger();

	void GetAmmo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

		void Reload();

	UPROPERTY(VisibleAnywhere)
		float Ammo;
	UPROPERTY(EditAnywhere)
		float MaxMagazine = 30;
	UPROPERTY(EditAnywhere)
		float MaxAmmo = 30;
		
	//Using to hide the mesh when switch guns
	class USkeletalMeshComponent* GetGunMesh() { return Mesh;}

private:

	bool GunTrace(FHitResult &Hit, FVector& ShotDirection);

	AController* GetOwnerController() const;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Blood;

	UPROPERTY(EditAnywhere, Category = "Info")
		float MaxRange = 1000;

	UPROPERTY(EditAnywhere, Category = "Info")
		float Damage = 10;

	UPROPERTY(EditAnywhere, Category = "Info")
		float HeadShotDamage = 50;

};
