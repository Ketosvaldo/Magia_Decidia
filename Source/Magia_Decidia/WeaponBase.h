// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "WeaponBase.generated.h"

UCLASS()
class MAGIA_DECIDIA_API AWeaponBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Physics, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Physics, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Physics, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	UFUNCTION()
	virtual void BeginPlay();
public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=ProjectileSettings)
	float Speed;
	UPROPERTY(EditAnywhere, Category=ProjectileSettings)
	float Gravity;
	UPROPERTY(EditAnywhere, Category=ProjectileSettings)
	bool bIsHoming;
	UPROPERTY(EditAnywhere, Category=ProjectileSettings)
	bool bShouldDestroy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ProjectileSettings)
    bool bIsItem;
	UPROPERTY(EditAnywhere, Category=Effects)
	UParticleSystem* ImpactEffect;
	UPROPERTY(EditAnywhere, Category=Sounds)
	USoundBase* ImpactSound;
	UPROPERTY(EditAnywhere, Category = Target)
	AActor* Target;

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable)
	void RotateToTarget() const;

	UFUNCTION(BlueprintImplementableEvent)
	void MakeDamage();
};