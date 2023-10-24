// Fill out your copyright notice in the Description page of Project Settings.
#include "WeaponBase.h"

#include "Magia_DecidiaCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OverlapBegin);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->ProjectileGravityScale = Gravity;
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->SetupAttachment(BoxCollision);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(BoxCollision);

	Target = nullptr;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	TSubclassOf<ACharacter> CharacterClass = ACharacter::StaticClass();
	TArray<AActor*> FoundCharacters;
	if(Target == nullptr)
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), CharacterClass, FoundCharacters);
		AActor* ActorTarget = nullptr;
		float ActualDistance = NULL;
		for(int i = 0; i < FoundCharacters.Num(); i++)
		{
			if(FoundCharacters[i] != GetOwner())
			{
				const float ActorDistance = GetDistanceTo(FoundCharacters[i]);
				if(ActorDistance < ActualDistance || !ActualDistance)
				{
					ActorTarget = FoundCharacters[i];
					ActualDistance = ActorDistance;
				}
			}
		}
		Target = ActorTarget;
		RotateToTarget();
	}
	BoxCollision->IgnoreActorWhenMoving(GetOwner(), true);
	if(bIsHoming)
	{
		ProjectileMovement->bIsHomingProjectile = bIsHoming;
		ProjectileMovement->HomingTargetComponent = Target->GetRootComponent();
	}
}

void AWeaponBase::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == this)
		return;
	const UWorld* World = GetWorld();
	if(World == nullptr)
		return;
	const FVector SpawnLocation = SweepResult.Location;
	UGameplayStatics::SpawnEmitterAtLocation(World, ImpactEffect, SpawnLocation);
	UGameplayStatics::SpawnSoundAtLocation(World, ImpactSound, SpawnLocation);
	Destroy();
}

void AWeaponBase::RotateToTarget() const
{
	const FVector PlayerLocation = GetActorLocation();
	const FVector TargetLocation = Target->GetActorLocation();
	const FVector Velocity = FVector(TargetLocation - PlayerLocation).GetSafeNormal();
	ProjectileMovement->SetVelocityInLocalSpace(Velocity * Speed);
}
