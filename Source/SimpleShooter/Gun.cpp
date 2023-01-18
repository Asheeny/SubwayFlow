// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "SimpleShooter/ShooterCharacter.h"
#include "Math/UnrealMathUtility.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	FireRateTimer = 0;
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (FireRateTimer > 0)
	{
		FireRateTimer -= DeltaTime;
	}
}

void AGun::PullTrigger(bool& HitEnemy, bool& CriticalHit)
{
	if (FireRateTimer > 0)
		return;
	FireRateTimer = FireRate;

	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
		return;
	
	FHitResult Hit;
	FVector ShotDirection;
	bool bHasHit = GunTrace(Hit, ShotDirection, OwnerController);

	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	if (bHasHit)
	{
		AActor* Target = Hit.GetActor();
		if (Target != nullptr)
		{
			if (Target->ActorHasTag(TEXT("Enemy")) && !OwnerController->ActorHasTag(TEXT("Enemy")))
			{
				HitEnemy = true;

				if (Hit.BoneName.IsEqual((TEXT("head"))))
				{
					SpawnImpact(CriticalImpactEffect, CriticalImpactSound, Hit, ShotDirection);

					CriticalHit = true;
				}
				else {
					SpawnImpact(ImpactEffect, ImpactSound, Hit, ShotDirection);

					CriticalHit = false;
				}
			}
			else {
				SpawnImpact(ImpactEffect, ImpactSound, Hit, ShotDirection);

				HitEnemy = false;
				CriticalHit = false;
				//UE_LOG(LogTemp, Warning, TEXT("Accuracy %f"), MyAccuracy());
			}
		}
		FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
		Target->TakeDamage(Damage, DamageEvent, OwnerController, this);
	}
}

float AGun::MyAccuracy()
{
	return Accuracy;
}

float AGun::CalculateAccuracy()
{
	AShooterCharacter* Player = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	return BulletSpread / (1 / Player->GetAccuracy() + 1);
}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection, AController* OwnerController)
{
	FVector ViewLocation;
	FRotator ViewRotation;
	OwnerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

	FCollisionQueryParams IgnoreParams;
	IgnoreParams.AddIgnoredActor(this);
	IgnoreParams.AddIgnoredActor(GetOwner());

	ECollisionChannel Channel = ECollisionChannel::ECC_GameTraceChannel1;
	ShotDirection = -ViewRotation.Vector();
	FRotator RotatorAccuracyApplied = ViewRotation;

	if (ActorHasTag(TEXT("Enemy")))
	{
		Accuracy = CalculateAccuracy();
		Channel = ECollisionChannel::ECC_GameTraceChannel2;

		RotatorAccuracyApplied.Pitch += FMath::RandRange(-Accuracy, Accuracy);
		RotatorAccuracyApplied.Yaw += FMath::RandRange(-Accuracy, Accuracy);
	}

	FVector End = (RotatorAccuracyApplied.Vector() * MaxRange) + ViewLocation;

	return GetWorld()->LineTraceSingleByChannel(Hit, ViewLocation, End, Channel, IgnoreParams);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
		return nullptr;
	return OwnerPawn->GetController();
}

void AGun::SpawnImpact(UParticleSystem* CurrentImpactEffect, USoundBase* CurrentImpactSound, FHitResult Hit, FVector ShotDirection)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CurrentImpactEffect, Hit.ImpactPoint, ShotDirection.Rotation(), true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), CurrentImpactSound, Hit.Location);
}

