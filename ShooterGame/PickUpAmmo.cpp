// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpAmmo.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
APickUpAmmo::APickUpAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	Collision->InitBoxExtent(FVector(16.f, 32.f, 16.f));
	Collision->SetCollisionProfileName("Trigger");
	RootComponent = Collision;

	MeshBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshBox->SetRelativeLocation(FVector(0.f, 0.f, 16.f));
	MeshBox->SetupAttachment(RootComponent);

	PickUpSound = CreateDefaultSubobject<UAudioComponent>(TEXT("PickUp Sound"));
	PickUpSound->bAutoActivate = false;
	PickUpSound->SetupAttachment(RootComponent);

	Collision->OnComponentBeginOverlap.AddDynamic(this, &APickUpAmmo::PickCollision);
}

// Called when the game starts or when spawned
void APickUpAmmo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickUpAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickUpAmmo::PickCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AShootherChar* Char = Cast<AShootherChar>(OtherActor);

	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor == Char))
	{
		PickUpSound->Activate(true);
		PickUpSound->bStopWhenOwnerDestroyed = false;
		PickUpSound->Play(0.0f);
		Destroy();

		Char->GetAmmo();
	}
}

