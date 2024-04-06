// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "Engine/World.h"
#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
    if (PhysicsHandle == nullptr) {
		return;
	}
    if (PhysicsHandle->GetGrabbedComponent() != nullptr) {
	FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
    PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const {
 
	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	bool HasHit = GetWorld()->SweepSingleByChannel(OutHitResult,
	 Start, End, FQuat::Identity,
	 ECC_GameTraceChannel2, Sphere);
    return HasHit;
}

void UGrabber::Grab() {

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
    if (PhysicsHandle == nullptr) {
		return;
	}

	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
    const float SphereRadius = 10.0f; // Size of the sphere in Unreal units (cm)
    const int32 Segments = 12; // Complexity of the sphere's mesh, more segments = smoother sphere
    const FColor SphereColor = FColor::Red; // Color of the sphere
    const float Thickness = 0.0f; // Thickness of the lines. Use a small positive value for thicker lines

  // DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, SphereRadius, Segments, FColor::Green, true,5, 0,Thickness);

	FHitResult HitResult;
    
     bool HasHit = GetGrabbableInReach(HitResult);

	 if (HasHit) {
		// DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, SphereRadius, Segments, FColor::Green, true,5, 0,Thickness);

		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		AActor* HitActor = HitResult.GetActor();
	    HitActor->Tags.Add("Grabbed");
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitResult.GetComponent(),
			NAME_None,
			HitResult.ImpactPoint,
			HitResult.GetComponent()->GetComponentRotation()
		);
	 } 
	 
	
}


void UGrabber::Release() {
UPhysicsHandleComponent* PhysicsComponent = GetPhysicsHandle();
if (PhysicsComponent == nullptr) {
		return;
	}
if (PhysicsComponent->GetGrabbedComponent() != nullptr) {
	PhysicsComponent->GetGrabbedComponent()->GetOwner()->Tags.Remove("Grabbed");
	PhysicsComponent->GetGrabbedComponent()->WakeAllRigidBodies();
    PhysicsComponent->ReleaseComponent();
}

	
}


UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const {
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Result == nullptr) {
		//UE_LOG(LogTemp, Error, TEXT("No such component! %s"), *GetOwner()->GetName());
	}
	return Result;
}


