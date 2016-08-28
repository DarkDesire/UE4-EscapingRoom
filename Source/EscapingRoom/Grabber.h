// Copyright Eldar Nezametdinov 2016

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPINGROOM_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

  // How far ahead of the player can we reach
  UPROPERTY( EditAnywhere )
  float Reach = 100.f;

	
private:

  UPhysicsHandleComponent* PhysicsHandle = nullptr;

  UInputComponent* InputComponent = nullptr;

  // Ray-cast and grab what's in reach
  void Grab();

  // Called when grab is released
  void Release();

  // Find (assumed) attached physics handle component
  void FindPhysicsHandleComponent();

  // Setup (assumed) attached input component
  void SetupInputComponent();

  // Return hit for first physics body in reach
  const FHitResult GetFirstPhysicsBodyInReach();
};