// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void PullTrigger(bool& HitEnemy, bool& CriticalHit);
	float MyAccuracy();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
		UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere)
		USoundBase* MuzzleSound;
	UPROPERTY(EditAnywhere)
		UParticleSystem* ImpactEffect;		
	UPROPERTY(EditAnywhere)
		USoundBase* ImpactSound;		
	UPROPERTY(EditAnywhere)
		UParticleSystem* CriticalImpactEffect;
	UPROPERTY(EditAnywhere)
		USoundBase* CriticalImpactSound;	
	UPROPERTY(EditAnywhere)
		float MaxRange = 1000;		
	UPROPERTY(EditAnywhere)
		float Damage = 10;
	UPROPERTY(EditAnywhere)
		float BulletSpread = 0;	
	UPROPERTY(EditAnywhere)
		float FireRate = 0;

	float Accuracy = 0;
	float FireRateTimer = FireRate;

	float CalculateAccuracy();
	bool GunTrace(FHitResult& Hit, FVector& ShotDirection, AController* OwnerController);
	AController* GetOwnerController() const;
	
	void SpawnImpact(UParticleSystem* CurrentImpactEffect, USoundBase* CurrentImpactSound, FHitResult Hit, FVector ShotDirection);
};
