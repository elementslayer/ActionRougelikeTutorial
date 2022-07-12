// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMagicProjectileBase.h"
#include "SMagicProjectileSingularity.generated.h"


class URadialForceComponent;
class USphereComponent;

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API ASMagicProjectileSingularity : public ASMagicProjectileBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASMagicProjectileSingularity();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Force Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URadialForceComponent* ForceComp;

	//Force Other Actor To Delete Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* RemovalComp;

	//Overlap Component
	UFUNCTION()
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherCOmp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FTimerHandle TimerHandle_SingularityDuration;

	void SingularityFinished();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
