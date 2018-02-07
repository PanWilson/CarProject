// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyCustomCar.generated.h"

UCLASS()
class CARPROJECT_API AMyCustomCar : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyCustomCar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* CarBody;

	//Input funtions
	void MoveForward(float AxisValue);
	void RotateRight(float AxisValue);
	void Break(float AxisValue);

	//Input variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
		float Acceleration = 1500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
		float MaxTurnDegre = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
		float AxelDistance=370;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
		float DragConst = 0.2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
		float RollingR = 12;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
		float BreakingConstat=800;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
		float AntiSlide=3500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
		float MaxRightMCOffset=40;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
		float MaxForwardMCOffset=40;


	//Helping variables
	
	FVector PVelocity;		//Velocity projected on a car horizontal plane
	
	FVector LPVelocity = FVector(0,0,0);		//Last tick projected velocity

	//Movement variables
	UPROPERTY(BlueprintReadWrite)
	float Speed;
	UPROPERTY(BlueprintReadWrite)
		bool FrontAxelTouching;
	UPROPERTY(BlueprintReadWrite)
		bool BackAxelTouching;

	// Calculation
	void CalcualteSpeed();
	void CalculateMassTransfer();
	FVector CalculateAntiSlide();
	FVector CalculateDrag();
	FVector CalculateFriction();
};
