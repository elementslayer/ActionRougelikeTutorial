// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "DrawDebugHelpers.h"
#include "SInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"



// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	//Allow the character to not follow the camera directly
	bUseControllerRotationYaw = false;

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Visualize the rotation of the player
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	//Offset to the right of the pawn
	LineStart += GetActorRightVector() * 100.0f;
	//Set line in direction of the actors forward vector
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	//Draw Actors Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	//Draw Controller rotation (Player controller that posssesed this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SingularityAttack", IE_Pressed, this, &ASCharacter::SingularityAttack);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);

}

FVector ASCharacter::FindTraceEnd()
{

	//Get the correct rotation
	FRotator CurrentRotation = GetControlRotation();
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FVector CameraLocation = CameraComp->GetComponentLocation();
	FVector CameraRotation = CameraComp->GetComponentRotation().Vector();

	FVector TraceEndLocation = CameraLocation + (CameraRotation * 1000.f);

	DrawDebugLine(GetWorld(), HandLocation, TraceEndLocation, FColor::Yellow, false, 2.0f, 0, 2.0f);

	return TraceEndLocation;
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	//XForward
	//YRight
	//ZUp

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}


void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}


void ASCharacter::SingularityAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::SingularityAttack_TimeElapsed, 0.2f);
}

void ASCharacter::SingularityAttack_TimeElapsed()
{
	if (ensure(SingularityClass))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		//Trace Object Params
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

		FVector TraceEnd = ASCharacter::FindTraceEnd();


		FHitResult HitResult;
		bool bHitResult = GetWorld()->LineTraceSingleByObjectType(HitResult, HandLocation, TraceEnd, ObjectQueryParams);
		//DrawDebugLine(GetWorld(), HandLocation, TraceEnd, FColor::Yellow, false, 2.0f, 0, 2.0f);


		if (bHitResult)
		{
			TraceEnd = HitResult.ImpactPoint;
		}

		FRotator AdjustedRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTM = FTransform(AdjustedRotation, HandLocation);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		GetWorld()->SpawnActor<AActor>(SingularityClass, SpawnTM, SpawnParams);
	}
}


void ASCharacter::PrimaryAttack_TimeElapsed()
{
	if (ensure(ProjectileClass))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		//Trace Object Params
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

		FVector TraceEnd = ASCharacter::FindTraceEnd();


		FHitResult HitResult;
		bool bHitResult = GetWorld()->LineTraceSingleByObjectType(HitResult, HandLocation, TraceEnd, ObjectQueryParams);
		//DrawDebugLine(GetWorld(), HandLocation, TraceEnd, FColor::Yellow, false, 2.0f, 0, 2.0f);


		if(bHitResult)
		{
			TraceEnd = HitResult.ImpactPoint;
		}

		FRotator AdjustedRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTM = FTransform(AdjustedRotation, HandLocation);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}
}

void ASCharacter::PrimaryInteract()
{

	if(InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
	
}
