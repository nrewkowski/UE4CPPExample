// Fill out your copyright notice in the Description page of Project Settings.
#pragma warning( disable : 4996 )

#include "VRCPPPawn.h"

// Sets default values
AVRCPPPawn::AVRCPPPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRTrackingCenter = CreateDefaultSubobject<USceneComponent>(TEXT("VRTrackingCenter"));
	Head = CreateDefaultSubobject<UCameraComponent>(TEXT("Head"));
	outputText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("outputText"));

	LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftController"));
	raygun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("raygun"));
	LeftCone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftCone"));

	RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightController"));
	RightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightMesh"));

	
	Head->SetupAttachment(VRTrackingCenter);
	outputText->SetupAttachment(Head);
	LeftController->SetupAttachment(VRTrackingCenter);
	raygun->SetupAttachment(LeftController);
	LeftCone->SetupAttachment(raygun);
	RightController->SetupAttachment(VRTrackingCenter);
	RightMesh->SetupAttachment(RightController);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> loadedObj(TEXT("StaticMesh'/Game/__rayguns/raygun.raygun'"));
	this->raygun->SetStaticMesh(loadedObj.Object);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> cone(TEXT("StaticMesh'/Engine/BasicShapes/Cone.Cone'"));
	this->LeftCone->SetStaticMesh(cone.Object);
	this->RightMesh->SetStaticMesh(cone.Object);

	static ConstructorHelpers::FObjectFinder<UMaterial> graymat(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));

	LeftCone->SetMaterial(0, graymat.Object);
	RightMesh->SetMaterial(0, graymat.Object);

	RightController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	LeftController->MotionSource = FXRMotionControllerBase::LeftHandSourceId;
	
	
	raygun->SetRelativeLocation(FVector(15, 0, 0));
	LeftCone->SetRelativeLocation(FVector(26, 0, 6));
	LeftCone->SetRelativeRotation(FRotator(-90, 180, 540));
	LeftCone->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));

	RightMesh->SetRelativeRotation(FRotator(-90, 180, 180));
	RightMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));

	this->RootComponent = VRTrackingCenter;

	AutoPossessPlayer = EAutoReceiveInput::Player0;


	static ConstructorHelpers::FObjectFinder<UMaterial> unlitText(TEXT("Material'/Engine/EngineMaterials/DefaultTextMaterialTranslucent.DefaultTextMaterialTranslucent'"));
	outputText->SetMaterial(0, unlitText.Object);
	outputText->SetTextRenderColor(FColor::Red);
	outputText->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	outputText->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;
	outputText->SetRelativeRotation(FRotator(0, 180.0f, 0));
	outputText->SetRelativeLocation(FVector(150, 0, 0));

	raygun->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftCone->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	LeftCone->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RightMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	navmesh = dynamic_cast<ARecastNavMesh*>(UGameplayStatics::GetActorOfClass(GetWorld(), ARecastNavMesh::StaticClass()));
	
}

// Called when the game starts or when spawned
void AVRCPPPawn::BeginPlay()
{
	Super::BeginPlay();
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
}

// Called every frame
void AVRCPPPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVRCPPPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("GoToThere", EInputEvent::IE_Pressed, this, &AVRCPPPawn::GoToThere);
	PlayerInputComponent->BindAction("ShootGun",EInputEvent::IE_Pressed, this, &AVRCPPPawn::ShootGunPressed);
	PlayerInputComponent->BindAction("ShootGun", IE_Released, this, &AVRCPPPawn::ShootGunReleased);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AVRCPPPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AVRCPPPawn::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AVRCPPPawn::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AVRCPPPawn::TurnUp);

	PlayerInputComponent->BindAction("Grip", IE_Pressed, this, &AVRCPPPawn::GripPressed);
	PlayerInputComponent->BindAction("Grip", IE_Released, this, &AVRCPPPawn::GripReleased);

	PlayerInputComponent->BindAction("ShootAndGrabNoSnap", IE_Pressed, this, &AVRCPPPawn::ShootAndGrabNoSnapPressed);
	PlayerInputComponent->BindAction("ShootAndGrabNoSnap", IE_Released, this, &AVRCPPPawn::ShootAndGrabNoSnapReleased);

	PlayerInputComponent->BindAction("ShootAndGrabSnap", IE_Pressed, this, &AVRCPPPawn::ShootAndGrabSnapPressed);
	PlayerInputComponent->BindAction("ShootAndGrabSnap", IE_Released, this, &AVRCPPPawn::ShootAndGrabSnapReleased);

	PlayerInputComponent->BindAction("MagneticGrip", IE_Pressed, this, &AVRCPPPawn::MagneticGripPressed);
	PlayerInputComponent->BindAction("MagneticGrip", IE_Released, this, &AVRCPPPawn::MagneticGripReleased);
	
}

void AVRCPPPawn::MoveForward(float AxisValue) {
	this->AddActorWorldOffset(this->GetActorForwardVector()*AxisValue*translationSpeed);
}

void AVRCPPPawn::MoveRight(float AxisValue) {
	this->AddActorWorldOffset(this->GetActorRightVector()*AxisValue*translationSpeed);
}

void AVRCPPPawn::Turn(float AxisValue) {
	this->AddActorWorldRotation(FRotator(0,AxisValue*rotationalSpeed,0));
}

void AVRCPPPawn::TurnUp(float AxisValue) {
	this->AddActorLocalRotation(FRotator(-AxisValue* rotationalSpeed, 0, 0));
}

void AVRCPPPawn::ShootGunPressed() {
	this->outputText->SetText("Shoot gun");
	if (thingOnGun) {
		thingOnGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		thingOnGun->GetStaticMeshComponent()->SetSimulatePhysics(true);
		thingOnGun->GetStaticMeshComponent()->AddImpulse(LeftCone->GetUpVector() * 10000);
		thingOnGun = nullptr;
	} else {
		const FVector loc = LeftCone->GetComponentLocation();
		const FRotator rot = UKismetMathLibrary::MakeRotFromX(LeftCone->GetUpVector());
		ABullet* newBullet = (ABullet*)this->GetWorld()->SpawnActor(ABullet::StaticClass(), &loc, &rot);

		newBullet->bulletMesh->AddImpulse(LeftCone->GetUpVector() * 200000);
	}
}

void AVRCPPPawn::ShootGunReleased() {
	this->outputText->SetText("release gun");
}

void AVRCPPPawn::GoToThere() {
	GLog->Log("telepor");
	this->outputText->SetText("telepor");
	
	FHitResult hit;
	TArray<AActor*> ignored;
	ignored.Add(this);

	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), RightController->GetComponentLocation(),
		RightController->GetComponentLocation() + (RightMesh->GetUpVector()*10000.0f), UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_WorldStatic),false,ignored,
		EDrawDebugTrace::Persistent,hit,true)){

		if (hit.GetActor()) {
			this->outputText->SetText(hit.GetActor()->GetName());
		}

		//this->SetActorLocation(hit.ImpactPoint);
		FNavLocation outnav;
		if (navmesh->ProjectPoint(hit.ImpactPoint,outnav,FVector(1000,1000,1000))) {
			SetActorLocation(outnav.Location);
		}
	}
}

void AVRCPPPawn::GripPressed() {
	this->outputText->SetText("grip");
	TArray<AActor*> outoverlap;
	RightMesh->GetOverlappingActors(outoverlap);

	if (outoverlap.Num() > 0) {
		this->outputText->SetText(outoverlap[0]->GetName());
		ACollectible* trycast = dynamic_cast<ACollectible*>(outoverlap[0]);
		if (trycast) {
			this->outputText->SetText("succeed");
			trycast->GetStaticMeshComponent()->SetSimulatePhysics(false);
			//trycast->GetStaticMeshComponent()->SetEnableGravity(false);
			trycast->AttachToComponent(RightController, FAttachmentTransformRules::KeepWorldTransform);

			thingIGrabbed = trycast;

			if (thingOnGun == trycast) {
				thingOnGun = nullptr;
			}
		}
	}
}

void AVRCPPPawn::GripReleased() {
	this->outputText->SetText("grip release");

	letGo();
}

void AVRCPPPawn::ShootAndGrabNoSnapPressed() {
	this->outputText->SetText("nosnap");

	ForceGrab(false);

}

void AVRCPPPawn::ForceGrab(bool shouldSnap) {
	FHitResult hit;
	TArray<AActor*> ignored;
	ignored.Add(this);
	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), RightController->GetComponentLocation(),
		RightController->GetComponentLocation() + (RightMesh->GetUpVector()*10000.0f), UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_WorldStatic), false, ignored,
		EDrawDebugTrace::Persistent, hit, true)) {
		if (hit.GetActor()) {
			ACollectible* trycast = dynamic_cast<ACollectible*>(hit.GetActor());
			if (trycast) {
				this->outputText->SetText(hit.GetActor()->GetName());
				trycast->GetStaticMeshComponent()->SetSimulatePhysics(false);
				trycast->AttachToComponent(RightController, shouldSnap ? FAttachmentTransformRules::SnapToTargetNotIncludingScale : FAttachmentTransformRules::KeepWorldTransform);

				thingIGrabbed = trycast;

				if (thingOnGun == trycast) {
					thingOnGun = nullptr;
				}
			}
		}
	}
}

void AVRCPPPawn::ShootAndGrabNoSnapReleased() {
	this->outputText->SetText("nosnap release");
	letGo();
}

void AVRCPPPawn::ShootAndGrabSnapPressed() {
	this->outputText->SetText("snap");

	ForceGrab(true);
}



void AVRCPPPawn::ShootAndGrabSnapReleased() {
	this->outputText->SetText("snap release");
	letGo();
}

void AVRCPPPawn::MagneticGripPressed() {
	this->outputText->SetText("mag");

	TArray<FHitResult> hits;
	TArray<AActor*> ignored;
	ignored.Add(this);

	TArray<TEnumAsByte<EObjectTypeQuery>> objTypes;

	objTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	if (UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), RightMesh->GetComponentLocation(), RightMesh->GetComponentLocation() + FVector(0, 0, 0.1f), 1000, objTypes, false,
		ignored, EDrawDebugTrace::Persistent, hits, true)) {
		ACollectible* closestObject = getClosestHitObject(hits);

		closestObject->GetStaticMeshComponent()->SetSimulatePhysics(false);
		closestObject->AttachToComponent(RightController, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		thingIGrabbed = closestObject;

		if (thingOnGun == closestObject) {
			thingOnGun = nullptr;
		}
	}

}

ACollectible* AVRCPPPawn::getClosestHitObject(TArray<FHitResult> hits) {
	float closestDistance = 100000000.0f;
	ACollectible* closestObject = nullptr;

	for (FHitResult hit : hits) {
		ACollectible* trycast = dynamic_cast<ACollectible*>(hit.GetActor());
		if (trycast) {
			if ((RightMesh->GetComponentLocation() - trycast->GetActorLocation()).Size() < closestDistance) {
				closestDistance = (RightMesh->GetComponentLocation() - trycast->GetActorLocation()).Size();
				closestObject = trycast;
			}
		}
	}

	return closestObject;
}


void AVRCPPPawn::MagneticGripReleased() {
	this->outputText->SetText("mag release");
	letGo();
}

void AVRCPPPawn::letGo() {
	if (thingIGrabbed) {
		if (LeftCone->IsOverlappingActor(thingIGrabbed)) {
			if (thingOnGun) {
				thingOnGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				thingOnGun->GetStaticMeshComponent()->SetSimulatePhysics(true);
			}

			thingIGrabbed->GetStaticMeshComponent()->SetSimulatePhysics(false);
			thingIGrabbed->AttachToComponent(LeftCone, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			thingOnGun = thingIGrabbed;
			thingIGrabbed = nullptr;
		}
		else {
			thingIGrabbed->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			thingIGrabbed->GetStaticMeshComponent()->SetSimulatePhysics(true);
			thingIGrabbed = nullptr;
		}
	}
}