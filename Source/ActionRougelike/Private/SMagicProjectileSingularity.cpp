// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectileSingularity.h"

#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

ASMagicProjectileSingularity::ASMagicProjectileSingularity()
{
	PrimaryActorTick.bCanEverTick = true;

	RemovalComp = CreateDefaultSubobject<USphereComponent>("RemovalComp");
	RemovalComp->SetupAttachment(RootComponent);

	RemovalComp->SetSphereRadius(16.0f);
	RemovalComp->SetGenerateOverlapEvents(true);

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(RootComponent);

	ForceComp->SetAutoActivate(true);
	ForceComp->Radius = 500.0f;
	ForceComp->ImpulseStrength = 2500.0f;
	ForceComp->bImpulseVelChange = true;
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);

}

void ASMagicProjectileSingularity::BeginPlay()
{

	Super::BeginPlay();
	//On overlap of another actor within the sphere component, make sure to destory the actor

	GetWorldTimerManager().SetTimer(TimerHandle_SingularityDuration, this, &ASMagicProjectileSingularity::SingularityFinished, 5.0f);
	RemovalComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectileSingularity::OnActorBeginOverlap);
}

void ASMagicProjectileSingularity::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherCOmp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OtherActor->Destroy();
}


void ASMagicProjectileSingularity::SingularityFinished()
{
	Destroy();
}

void ASMagicProjectileSingularity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
