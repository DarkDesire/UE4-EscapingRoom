// Copyright Eldar Nezametdinov 2016

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FDoorEvent );

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPINGROOM_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

  UPROPERTY( BlueprintAssignable )
  FDoorEvent OnOpen;

  UPROPERTY( BlueprintAssignable )
  FDoorEvent OnClose;

private:
  // Returns total mass in kg
  float GetTotalMassOfActorsOnPlate();

  UPROPERTY( EditAnywhere )
  ATriggerVolume* PressurePlate = nullptr;

  UPROPERTY( EditAnywhere )
  float MassNeedForOpenDoor = 30.f;

  UPROPERTY( VisibleAnywhere )
  float LastDoorOpenTime;
};
