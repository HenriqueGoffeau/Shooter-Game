// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

UCLASS()
class SHOOTERGAME_API AGrenade : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle Timer;

	UFUNCTION()
		void Explode();



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Throw(FVector ForwardVector);

private:

	UPROPERTY(EditAnywhere)
		USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere)
		UParticleSystem* Explosion;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* Collision;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		class USphereComponent* SphereDamage;

	UPROPERTY(EditAnywhere)
		int32 Damage = 200;

	UPROPERTY(EditAnywhere)
		float ThrowForce = 500.f;
};