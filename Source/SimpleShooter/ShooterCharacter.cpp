// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooter/SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AShooterCharacter::IsDead() const
{
	return CurrentHealth <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return CurrentHealth / MaxHealth;
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);

	//PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("PrimaryFire"), EInputEvent::IE_Pressed, this, &AShooterCharacter::UsePrimaryFire);
	PlayerInputComponent->BindAction(TEXT("PrimaryFire"), EInputEvent::IE_Released, this, &AShooterCharacter::ReleasedTrigger);
	//PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AShooterCharacter::UseInteract);
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (IsDead())
		return 0;
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(CurrentHealth, DamageToApply);
	CurrentHealth -= DamageToApply;

	GetMesh()->SetEnableGravity(false);
	if (IsDead())
	{
		ASimpleShooterGameModeBase* CurrentGameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();

		if (CurrentGameMode != nullptr)
		{
			CurrentGameMode->PawnKilled(this);
		}
		
		//Gun->Destroy();
		Gun->SetLifeSpan(2);
		GetMesh()->SetRenderCustomDepth(false);
		DetachFromControllerPendingDestroy();
	}
	return CurrentHealth;
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

AGun* AShooterCharacter::GetGun()
{
	return Gun;
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::UsePrimaryFire()
{
	bool HitEnemy;
	bool CriticalHit;
	Gun->PullTrigger(HitEnemy, CriticalHit);

	//UE_LOG(LogTemp, Warning, TEXT("HitEnemy: %s"), HitEnemy? TEXT("true") : TEXT("false"));

	bShot = true;
	ShotsFired++;
	if (HitEnemy)
	{
		ShotsHit++;
		if (CriticalHit) {
			CurrentHealth = FMath::Min(CurrentHealth + HealthRegenOnCrit, MaxHealth);
		}
	}
}

void AShooterCharacter::ReleasedTrigger()
{
	bShot = false;
}

float AShooterCharacter::GetAccuracy()
{
	if (IsDead())
		return 0;
	if (ShotsFired == 0)
		return 0;
	return ShotsHit / ShotsFired;
}
