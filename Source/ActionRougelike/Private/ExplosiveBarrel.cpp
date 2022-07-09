// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	//MeshComp->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::OnHit);
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AExplosiveBarrel::OnOverlap);
	UE_LOG(LogTemp, Display, TEXT("Hi I am here"));
}



// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosiveBarrel::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyInteger, bool FromSweep, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Display, TEXT("OnOverlap"));
	OverlappedComponent->AddRadialImpulse(OverlappedComponent->GetComponentLocation(), 100.0f, 10000.0f, ERadialImpulseFalloff::RIF_Constant, false);
	
}

