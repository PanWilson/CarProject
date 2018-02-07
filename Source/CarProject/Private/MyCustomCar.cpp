// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCustomCar.h"
#include "Classes/Components/BoxComponent.h"
#include "Classes/Components/InputComponent.h"
#include "Engine/World.h"


// Sets default values
AMyCustomCar::AMyCustomCar()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CarBody = CreateDefaultSubobject<UBoxComponent>(TEXT("CarBody"));
	RootComponent = CarBody;
	CarBody->SetSimulatePhysics(true);
	CarBody->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);

}

// Called when the game starts or when spawned
void AMyCustomCar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCustomCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Projected velocity
	PVelocity = FVector::VectorPlaneProject(CarBody->GetComponentVelocity(), CarBody->GetUpVector());

	CalcualteSpeed();
	CalculateMassTransfer();

	LPVelocity = PVelocity;
}

// Called to bind functionality to input
void AMyCustomCar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Binding basic input
	InputComponent->BindAxis("MoveForward", this, &AMyCustomCar::MoveForward);
	InputComponent->BindAxis("Rotate", this, &AMyCustomCar::RotateRight);
	InputComponent->BindAxis("Breaking", this, &AMyCustomCar::Break);
}

void AMyCustomCar::MoveForward(float AxisValue)
{
		FVector Engine;
		if (!BackAxelTouching || (PVelocity.Size()<10 && InputComponent->GetAxisValue("Breaking"))) Engine = FVector(0, 0, 0);
		else if ((AxisValue < 0 && FVector::DotProduct(CarBody->GetForwardVector(), PVelocity.GetSafeNormal()) > 0) || InputComponent->GetAxisValue("Breaking")) {
			Engine = CarBody->GetForwardVector() * BreakingConstat * CarBody->GetMass() * -1 * copysign(1, FVector::DotProduct(CarBody->GetForwardVector(), PVelocity.GetSafeNormal()));
		}
		else Engine = CarBody->GetForwardVector() * CarBody->GetMass() * Acceleration * AxisValue;

		FVector Force = Engine - CalculateFriction() - CalculateDrag() - CalculateAntiSlide();

		CarBody->AddForce(Force, NAME_None, false);
}


void AMyCustomCar::RotateRight(float AxisValue)
{
	if (AxisValue && FrontAxelTouching) {
		float TurnRadius = AxelDistance / sin(MaxTurnDegre*AxisValue*(PI/180));
		float AngularVelocity = Speed/TurnRadius;

		CarBody->AddTorqueInRadians(CarBody->GetUpVector() * AngularVelocity * copysign(1, FVector::DotProduct(CarBody->GetForwardVector(), PVelocity.GetSafeNormal())), NAME_None,true);
	}

}

void AMyCustomCar::Break(float AxisValue)
{

}


void AMyCustomCar::CalcualteSpeed() {
	Speed = CarBody->GetComponentVelocity().Size();
}

void AMyCustomCar::CalculateMassTransfer() {

	//Calculate acceleration and project it on car plane
	FVector AccelerationV = (LPVelocity - PVelocity) / pow(GetWorld()->DeltaTimeSeconds, 2);
	FVector PAcceleration = FVector::VectorPlaneProject(AccelerationV, CarBody->GetUpVector());

	//Calculate offsets base on acceleration
	float RightOffset = MaxRightMCOffset * (Speed /3800 ) * FVector::DotProduct(PAcceleration.GetSafeNormal(),CarBody->GetRightVector());
	float ForwardOffset = MaxForwardMCOffset * (Speed /3800) * FVector::DotProduct(PAcceleration.GetSafeNormal(), CarBody->GetForwardVector());

	FVector CenterOfMass = FVector(ForwardOffset+25, RightOffset,-50);

	CarBody->SetCenterOfMass(CenterOfMass);
}

FVector AMyCustomCar::CalculateAntiSlide() {

	return CarBody->GetRightVector() * CarBody->GetMass() * FVector::DotProduct(CarBody->GetRightVector(), PVelocity.GetSafeNormal()) * AntiSlide;

}

FVector AMyCustomCar::CalculateDrag() {

	return DragConst * PVelocity * Speed;

}

FVector AMyCustomCar::CalculateFriction() {

	return RollingR * PVelocity;

}
