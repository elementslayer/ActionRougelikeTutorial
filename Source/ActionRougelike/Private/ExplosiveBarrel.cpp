// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"

#include "DrawDebugHelpers.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);

	//Applies small force on via tick component
	ForceComp->SetAutoActivate(false);
	ForceComp->Radius = 750.0f;
	ForceComp->ImpulseStrength  = 2500.0f; // Alternative 200000 if bImpluseVelChage = false
	//Ignores thje mass of other objects, force and radius much be much higher if this is false
	ForceComp->bImpulseVelChange = true;

	//Optial, default constructor of component already adds 4 object types to affect, excluding dynamic
	ForceComp->AddCollisionChannelToAffect((ECC_WorldDynamic));
}


void AExplosiveBarrel::PostInitializeComponents()
{
	//Call tot he parent
	Super::PostInitializeComponents();

	MeshComp->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::OnActorHit);
}

void AExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();
	UE_LOG(LogTemp, Display, TEXT("OnActorHit in ExplosiveBarrel"));
	UE_LOG(LogTemp, Warning, TEXT("OtherActor %s, at game time %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	
	FString CombinedString = FString::Printf(TEXT("Hit At Location %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);

}
