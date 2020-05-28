// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bulletMesh"));
	this->RootComponent = bulletMesh;

	static ConstructorHelpers::FObjectFinder<UMaterial> loadedRed(TEXT("/Game/red"));

	bulletMesh->SetMaterial(0, loadedRed.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> loadedObj(TEXT("/Engine/BasicShapes/Sphere"));
	bulletMesh->SetStaticMesh(loadedObj.Object);

	bulletMesh->SetWorldScale3D(FVector(0.2f, 0.1f, 0.1f));

	bulletMesh->SetSimulatePhysics(true);

	bulletMesh->SetMassOverrideInKg("Name_None", 100.0f);

	bulletMesh->SetNotifyRigidBodyCollision(true);
	bulletMesh->OnComponentHit.AddDynamic(this, &ABullet::EventHit);

	static ConstructorHelpers::FObjectFinder<UMaterial> loadedMaterial(TEXT("/Engine/EngineDebugMaterials/BoneWeightMaterial"));

	colorfulMaterial = loadedMaterial.Object;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::EventHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	//GLog->Log("bullet hit something");

	ACollectible* castedHit = dynamic_cast<ACollectible*>(OtherActor);

	if (castedHit) {
		float value = castedHit->treasureValue;

		castedHit->GetStaticMeshComponent()->SetMaterial(0, colorfulMaterial);
	}
}
