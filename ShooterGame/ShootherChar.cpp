
#include "ShootherChar.h"
#include "RifleGun.h"
#include "Grenade.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "ShooterGameGameModeBase.h"


// Sets default values
AShootherChar::AShootherChar()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShootherChar::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);

	GunIndex = 0;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	CurrentGun = GetWorld()->SpawnActor<ARifleGun>(GunClass, SpawnParams);
	if (CurrentGun)
	{
		GunInventory.Add(CurrentGun);
		CurrentGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	}
	if (ARifleGun* OtherGun = GetWorld()->SpawnActor<ARifleGun>(Gun2Class, SpawnParams))
	{
		OtherGun->GetGunMesh()->SetHiddenInGame(true);
		OtherGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		GunInventory.Add(OtherGun);
	}

	//if I want an 3° gun
	/*if (ARifleGun* OtherGun = GetWorld()->SpawnActor<ARifleGun>(Gun3Class, SpawnParams))
	{
		OtherGun->GetGunMesh()->SetHiddenInGame(true);
		OtherGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		GunInventory.Add(OtherGun);
	}*/

	Health = MaxHealth;

}


bool AShootherChar::IsDead() const
{
	return Health <= 0;
}

float AShootherChar::GetHealthPercent() const
{
	return Health / MaxHealth;
}

float AShootherChar::GetAmmoWeapon() const
{
	return CurrentGun->Ammo;
}

float AShootherChar::GetAmmoReserve() const
{
	return CurrentGun->MaxAmmo;
}

float AShootherChar::GetGrenadeAmount() const
{
	return GrenadeCap;
}

// Called every frame
void AShootherChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShootherChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveFoward", this, &AShootherChar::MoveFoward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShootherChar::MoveRight);

	//using the default pawn function to control the camera with the mouse						
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &APawn::AddControllerYawInput);
	//using the default pawn funtion to jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AShootherChar::Shoot);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AShootherChar::Reload);
	PlayerInputComponent->BindAction("SwitchNextWeapon", IE_Pressed, this, &AShootherChar::SwitchNextWeapon);
	PlayerInputComponent->BindAction("SwitchPreviousWeapon", IE_Pressed, this, &AShootherChar::SwitchPreviousWeapon);
	PlayerInputComponent->BindAction("ThrowGrenade", IE_Pressed, this, &AShootherChar::ThrowGrenade);
}

float AShootherChar::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;


	//checkin if is dead to prevent move and collision
	if (IsDead())
	{
		AShooterGameGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AShooterGameGameModeBase>();

		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}

		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return DamageToApply;
}

void AShootherChar::MoveFoward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);

}

void AShootherChar::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}


void AShootherChar::Shoot()
{
	CurrentGun->PullTrigger();
}

void AShootherChar::Reload()
{
	CurrentGun->Reload();
}

void AShootherChar::GetAmmo()
{
	CurrentGun->GetAmmo();
	GrenadeCap += 1;
}

void AShootherChar::ThrowGrenade()
{
	//Don't have throw anim so not going to hidde the gun at the moment
	bool CanThrow = GrenadeCap > 0;
	if (CanThrow)
	{
		CurrentGun->SetHidden(true);

		Grenade = GetWorld()->SpawnActor<AGrenade>(GrenadeClass);
			if (Grenade)
			{
				Grenade->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "hand_lSocket");
				Grenade->SetHidden(true);
			}

		Grenade->Throw(UKismetMathLibrary::GetForwardVector(GetControlRotation()));
		Grenade->SetHidden(false);
		--GrenadeCap;
	}
	CurrentGun->SetHidden(false);
}

void AShootherChar::SwitchNextWeapon()
{
	if (CurrentGun)
	{
		if (GunInventory.Num() > GunIndex + 1)
		{
			++GunIndex;
			if (ARifleGun* NextGun = GunInventory[GunIndex])
			{
				CurrentGun->GetGunMesh()->SetHiddenInGame(true);
				CurrentGun = NextGun;
				CurrentGun->GetGunMesh()->SetHiddenInGame(false);
			}
		}
	}
}

void AShootherChar::SwitchPreviousWeapon()
{
	if (CurrentGun)
	{
		if (GunIndex - 1 >= 0)
		{
			--GunIndex;
			if (ARifleGun* NextGun = GunInventory[GunIndex])
			{
				CurrentGun->GetGunMesh()->SetHiddenInGame(true);
				CurrentGun = NextGun;
				CurrentGun->GetGunMesh()->SetHiddenInGame(false);
			}
		}
	}
}
