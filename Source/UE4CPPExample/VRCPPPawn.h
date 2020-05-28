// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Core.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "MotionControllerComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "XRMotionControllerBase.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
//#include "GameFramework/Actor.h"
#include "Runtime/HeadMountedDisplay/Public/HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NavMesh/RecastNavMesh.h"
#include "Bullet.h"
#include "GameFramework/Pawn.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "VRCPPPawn.generated.h"

UCLASS()
class UE4CPPEXAMPLE_API AVRCPPPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRCPPPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = __hide)
	USceneComponent* VRTrackingCenter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = __hide)
	UCameraComponent* Head;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = __hide)
	UTextRenderComponent* outputText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = __hide)
	UMotionControllerComponent* LeftController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = __hide)
	UMotionControllerComponent* RightController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = __hide)
	UStaticMeshComponent* raygun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = __hide)
	UStaticMeshComponent* LeftCone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = __hide)
	UStaticMeshComponent* RightMesh;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Turn(float AxisValue);
	void TurnUp(float AxisValue);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = speeds)
	float translationSpeed = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = speeds)
	float rotationalSpeed = 1.0f;

	UFUNCTION(BlueprintCallable)
	void ShootGunPressed();
	UFUNCTION(BlueprintCallable)
	void ShootGunReleased();

	UFUNCTION(BlueprintCallable)
		void GoToThere();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
		ARecastNavMesh* navmesh;


	UFUNCTION(BlueprintCallable)
		void GripPressed();

	UFUNCTION(BlueprintCallable)
		void GripReleased();

	UFUNCTION(BlueprintCallable)
		void ShootAndGrabNoSnapPressed();

	UFUNCTION(BlueprintCallable)
		void ShootAndGrabNoSnapReleased();

	UFUNCTION(BlueprintCallable)
		void ShootAndGrabSnapPressed();	

	

	UFUNCTION(BlueprintCallable)
		void ShootAndGrabSnapReleased();

	UFUNCTION(BlueprintCallable)
		void MagneticGripPressed();

	UFUNCTION(BlueprintCallable)
		void MagneticGripReleased();

	UFUNCTION(BlueprintCallable)
		void letGo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
		ACollectible* thingIGrabbed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
		ACollectible* thingOnGun;

	void ForceGrab(bool shouldSnap);

	ACollectible* getClosestHitObject(TArray<FHitResult> hits);
};
