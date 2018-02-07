// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MySuspensionSpring.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CARPROJECT_API UMySuspensionSpring : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMySuspensionSpring();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	//Parameters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
		float SpringLenght;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
		float TireRadius;
	UPROPERTY(BlueprintReadOnly, Category = "Parameters")
		float Stiffnes;
	UPROPERTY(BlueprintReadOnly, Category = "Parameters")
		float Damper;
	UPROPERTY(BlueprintReadOnly, Category = "Parameters")
		bool OnGround;
	UPROPERTY(BlueprintReadOnly, Category = "Parameters")
		float Height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
		float DampingRatio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
		float MaxCompression;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
		TEnumAsByte<ECollisionChannel> Channel;

	//Helping variables
	AActor* ParentActor;
	UPrimitiveComponent* ParentPrimitive;
	float LastTickDepth = 0;

	//Calculate and apply force to parent
	UFUNCTION(BlueprintCallable)
		void CalculateForce();
};
