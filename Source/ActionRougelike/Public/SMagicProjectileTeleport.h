// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMagicProjectileBase.h"
#include "SMagicProjectileTeleport.generated.h"

class UParticleSystemComponent;

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API ASMagicProjectileTeleport : public ASMagicProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASMagicProjectileTeleport();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Force Other Actor To Delete Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* BlinkEffectComp;

	FTimerHandle TimerHandle_BlinkTravelDuration;
	FTimerHandle TimerHandle_BlinkDuration;

	void BlinkTravelFinished();
	void BlinkFinished();

	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
