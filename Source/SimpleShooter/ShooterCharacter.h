// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Components/InputComponent.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintPure)
		bool IsDead() const;
	UFUNCTION(BlueprintPure)
		float GetHealthPercent() const;		
	UFUNCTION(BlueprintCallable)
		float GetAccuracy();
	UFUNCTION(BlueprintCallable)
		virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(BlueprintReadWrite)
		bool bInRangeOfDoor = false;	
	UPROPERTY(BlueprintReadWrite)
		bool bInMeleeRange = false;
	UPROPERTY(BlueprintReadWrite)
		int CarsCleared = 0;
	UPROPERTY(BlueprintReadOnly)
		float CurrentHealth = 0;	
	UPROPERTY(BlueprintReadOnly)
		bool bShot;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void UsePrimaryFire();
	void ReleasedTrigger();

	AGun* GetGun();
private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	
	float ShotsFired = 0;
	float ShotsHit = 0;

	UPROPERTY(EditAnywhere)
		float RotationRate = 15;	
	UPROPERTY(EditDefaultsOnly)
		float MaxHealth = 100;		
	UPROPERTY(EditDefaultsOnly)
		float HealthRegenOnCrit = 0;	
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AGun> GunClass;
	UPROPERTY()
		AGun* Gun;
};
