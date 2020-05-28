// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectible.h"

ACollectible::ACollectible() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> loadedObj(TEXT("/Game/Geometry/Meshes/1M_Cube"));

	this->GetStaticMeshComponent()->SetStaticMesh(loadedObj.Object);

	this->GetStaticMeshComponent()->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f));

	static ConstructorHelpers::FObjectFinder<UMaterial> loadedMat(TEXT("/Game/Geometry/Meshes/CubeMaterial"));

	this->GetStaticMeshComponent()->SetMaterial(0, loadedMat.Object);
	this->GetStaticMeshComponent()->SetSimulatePhysics(true);
	this->SetMobility(EComponentMobility::Movable);

	this->GetStaticMeshComponent()->SetGenerateOverlapEvents(true);

	this->GetStaticMeshComponent()->SetNotifyRigidBodyCollision(true);
	this->GetStaticMeshComponent()->SetCollisionProfileName("PhysicsActor");

}