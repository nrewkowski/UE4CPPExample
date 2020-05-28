// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Engine/StaticMesh.h"
#include "Collectible.generated.h"

/**
 * 
 */
UCLASS()
class UE4CPPEXAMPLE_API ACollectible : public AStaticMeshActor
{
	GENERATED_BODY()
public:
		ACollectible();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
		float treasureValue = 0.0f;
};
