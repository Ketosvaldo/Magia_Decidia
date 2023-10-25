// Fill out your copyright notice in the Description page of Project Settings.
#include "WeaponBase.h"

#include "Magia_DecidiaCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
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

	Speed = 800.f;
	
	Target = nullptr;
	bShouldDestroy = true;
	bIsItem = false;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	if(bIsItem)
		return;
	const TSubclassOf<ACharacter> CharacterClass = ACharacter::StaticClass();
	TArray<AActor*> FoundCharacters;
	if(Target == nullptr)
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), CharacterClass, FoundCharacters);
		AActor* ActorTarget = nullptr;
		float ActualDistance = NULL;
		for(int i = 0; i < FoundCharacters.Num(); i++)
		{
			const float ActorDistance = GetDistanceTo(FoundCharacters[i]);
			if(ActorDistance < 65)
				MyActor = FoundCharacters[i];
			if(ActorDistance < ActualDistance && ActorDistance > 65 || !ActualDistance)
			{
				ActorTarget = FoundCharacters[i];
				ActualDistance = ActorDistance;
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
	if(bIsItem)
		return;
	if(OtherActor == MyActor)
		return;
	const UWorld* World = GetWorld();
	if(World == nullptr)
		return;
	const FVector SpawnLocation = SweepResult.Location;
	if(ImpactEffect != nullptr)
		UGameplayStatics::SpawnEmitterAtLocation(World, ImpactEffect, SpawnLocation);
	if(ImpactSound != nullptr)
		UGameplayStatics::SpawnSoundAtLocation(World, ImpactSound, SpawnLocation);
	MakeDamage(OtherActor);
	Destroy();
}

void AWeaponBase::RotateToTarget() const
{
	const FVector PlayerLocation = GetActorLocation();
	const FVector TargetLocation = Target->GetActorLocation();
	const FVector Velocity = FVector(TargetLocation - PlayerLocation).GetSafeNormal();
	ProjectileMovement->Velocity = Velocity * Speed;
	ProjectileMovement->bRotationFollowsVelocity = true;
}
