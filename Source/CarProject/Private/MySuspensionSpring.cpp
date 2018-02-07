// Fill out your copyright notice in the Description page of Project Settings.

#include "MySuspensionSpring.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UMySuspensionSpring::UMySuspensionSpring()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMySuspensionSpring::BeginPlay()
{
	Super::BeginPlay();

	SetTickGroup(TG_PrePhysics);
	ParentActor = GetAttachParent()->GetOwner();
	ParentPrimitive = Cast<UPrimitiveComponent>(ParentActor->GetComponentByClass(UPrimitiveComponent::StaticClass()));

	//Calculating spring parameters
	float SprungMass = ParentPrimitive->GetMass()/4;

	//stiffnes = sprung mass * gravity / max compresion
	Stiffnes = (SprungMass* abs(GetWorld()->GetGravityZ())) / MaxCompression;

	Damper = DampingRatio * 2 * sqrt(Stiffnes*SprungMass);
}


// Called every frame
void UMySuspensionSpring::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UMySuspensionSpring::CalculateForce();
}

void UMySuspensionSpring::CalculateForce() {

	if (ParentPrimitive) {

		//Collision setup
		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, ParentActor);
		RV_TraceParams.bTraceComplex = true;
		RV_TraceParams.bTraceAsyncScene = true;
		RV_TraceParams.bReturnPhysicalMaterial = false;

		FHitResult Hit(ForceInit);

		//Checking collision of tire
		GetWorld()->SweepSingleByChannel(
			Hit,
			GetComponentLocation(),
			GetComponentLocation() + GetUpVector()*-1 * SpringLenght,
			FQuat(0,0,0,0),
			Channel,
			FCollisionShape::MakeSphere(TireRadius),
			RV_TraceParams
		);

		if (Hit.bBlockingHit) {

			Height = Hit.Distance;
			OnGround = true;
			float depth = (SpringLenght - Hit.Distance);

			float suspensionForce = Stiffnes * depth + Damper * (depth - LastTickDepth)/GetWorld()->DeltaTimeSeconds;
			LastTickDepth = depth;
			ParentPrimitive->AddForceAtLocation(ParentPrimitive->GetUpVector()*suspensionForce, GetComponentLocation());

		}
		else {

			OnGround = false;
			Height = SpringLenght;
			LastTickDepth = 0;

		}
	}
}