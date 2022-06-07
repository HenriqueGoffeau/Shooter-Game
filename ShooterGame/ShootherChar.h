// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShootherChar.generated.h"

class ARifleGun;

UCLASS()
class SHOOTERGAME_API AShootherChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShootherChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	//to use in gamemode
	UFUNCTION(BlueprintPure)
		bool IsDead() const;

	//to use in the HUD
	UFUNCTION(BlueprintPure)
		float GetHealthPercent() const;
	UFUNCTION(BlueprintPure)
		float GetAmmoWeapon() const;
	UFUNCTION(BlueprintPure)
		float GetAmmoReserve() const;
	UFUNCTION(BlueprintPure)
		float GetGrenadeAmount() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Shoot();

	void Reload();

	void GetAmmo();

	void SwitchNextWeapon();
	void SwitchPreviousWeapon();

	void ThrowGrenade();

	UPROPERTY(EditAnywhere, Category = "Grenade")
		float GrenadeCap = 1;

private:

	void MoveFoward(float AxisValue);
	void MoveRight(float AxisValue);

	//Guns for the palyer
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ARifleGun> GunClass;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ARifleGun> Gun2Class;

	//If I want another gun
	/*UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ARifleGun> Gun3Class;*/

	UPROPERTY(EditAnywhere)
		TArray<ARifleGun*> GunInventory;

	//To use infos only about the current gun
	UPROPERTY()
		ARifleGun* CurrentGun;

	int32 GunIndex;

	UPROPERTY()
		class AGrenade* Grenade;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AGrenade> GrenadeClass;

	UPROPERTY(EditDefaultsOnly)
		float MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly)
		float Health;
};