// Copyright Eldar Nezametdinov 2016

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPINGROOM_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;

  void OpenDoor();
  void CloseDoor();
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
private:

  UPROPERTY( EditAnywhere )
  float OpenAngle = 90.f;

  UPROPERTY( EditAnywhere )
  ATriggerVolume* PressurePlate;

  UPROPERTY( EditAnywhere )
  float DoorCloseDelay = 2.f;

  UPROPERTY( VisibleAnywhere )
  float LastDoorOpenTime;

  UPROPERTY( VisibleAnywhere )
  AActor* ActorThatOpens; // Remember pawn inherits from actor

  AActor* Owner; // the door owning Actor

};
