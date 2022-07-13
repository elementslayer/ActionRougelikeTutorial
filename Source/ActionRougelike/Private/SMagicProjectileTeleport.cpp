// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectileTeleport.h"

#include "ExplosiveBarrel.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"

ASMagicProjectileTeleport::ASMagicProjectileTeleport()
{
	PrimaryActorTick.bCanEverTick = true;

	BlinkEffectComp = CreateDefaultSubobject<UParticleSystemComponent>("BlinkEffectComp");
	BlinkEffectComp->SetAutoActivate(false);
	BlinkEffectComp->SetupAttachment(RootComponent);
}

void ASMagicProjectileTeleport::BeginPlay()
{
	Super::BeginPlay();


	GetWorldTimerManager().SetTimer(TimerHandle_BlinkTravelDuration, this, &ASMagicProjectileTeleport::BlinkTravelFinished, 0.2f);
	SphereComp->OnComponentHit.AddDynamic(this, &ASMagicProjectileTeleport::OnActorHit);

}

void ASMagicProjectileTeleport::BlinkTravelFinished()
{
	GetWorldTimerManager().SetTimer(TimerHandle_BlinkDuration, this, &ASMagicProjectileTeleport::BlinkFinished, 0.2f);
	EffectComp->SetActive(false);
	MovementComp->SetActive(false);
	BlinkEffectComp->SetActive(true);
	UE_LOG(LogTemp, Display, TEXT("Blink Travel FInished"));
}

void ASMagicProjectileTeleport::BlinkFinished()
{
	
	APawn* InstigatorOwner= GetInstigator();

	FVector CurrentLocation = GetActorLocation();
	FRotator CurrentRotation = InstigatorOwner->GetActorRotation();

	InstigatorOwner->TeleportTo(CurrentLocation, CurrentRotation);

	Destroy();
	UE_LOG(LogTemp, Display, TEXT("Blink FInished"));
}

void ASMagicProjectileTeleport::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BlinkTravelDuration);
	GetWorldTimerManager().SetTimer(TimerHandle_BlinkDuration, this, &ASMagicProjectileTeleport::BlinkFinished, 0.2f);
	EffectComp->SetActive(false);
	MovementComp->SetActive(false);
	BlinkEffectComp->SetActive(true);
}

void ASMagicProjectileTeleport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

