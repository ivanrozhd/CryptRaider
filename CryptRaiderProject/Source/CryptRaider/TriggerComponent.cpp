// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent() 
{
  PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
    UE_LOG(LogTemp, Display, TEXT("Trigger Component alive!"));
	
}


	// Called every frame
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)  {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    AActor* Actor = GetAcceptableActor();

    if (Actor != nullptr) {
        UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
        if (Component!= nullptr) {
            Component->SetSimulatePhysics(false);   
        }
        Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
        SetShouldMove(true);
        UE_LOG(LogTemp, Error, TEXT("HEREEEEEEEEE!!!!"));
    }
    else 
    {
      SetShouldMove(false);
    }
    
 }


 AActor* UTriggerComponent::GetAcceptableActor() const 
 {
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);
    for (AActor* Actor : Actors) {
        bool HasAcceptableTag = Actor->ActorHasTag(AcceptableActorTag);
        bool HasGrabbed = Actor->ActorHasTag("Grabbed");
        if (HasAcceptableTag && !HasGrabbed) {
          return Actor;
        }   
    }
    return nullptr;
 }


 void  UTriggerComponent::SetMyMover(UMover* NewMover) {
    Mover = NewMover;
     if (Mover != nullptr) {
      
      UE_LOG(LogTemp, Warning, TEXT("Instanciated!!!!!!!!! %s"), *GetOwner()->GetName());

     }
 }

 void UTriggerComponent::SetShouldMove(bool ShouldMove) {
    if (Mover != nullptr) {
      Mover->ShouldMove = ShouldMove;
    }

 }