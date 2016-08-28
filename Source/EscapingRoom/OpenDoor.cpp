// Copyright Eldar Nezametdinov 2016

#include "EscapingRoom.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

  if ( !PressurePlate ) {
    UE_LOG( LogTemp, Error, TEXT( "%s missing pressure plate" ), *(GetOwner()->GetName()) );
  }
}

void UOpenDoor::OpenDoor()
{
  //GetOwner()->SetActorRotation( FRotator( 0.f, OpenAngle, 0.f ) );
  OnOpenRequest.Broadcast();
}

void UOpenDoor::CloseDoor()
{
  GetOwner()->SetActorRotation( FRotator( 0.f, 0.f, 0.f ) );
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

  float currentSeconds = GetWorld()->GetTimeSeconds();
	// Poll the Trigger Volume
  // If the ActorThatOpens is in the volume
  if ( GetTotalMassOfActorsOnPlate() > MassNeedForOpenDoor ) {
    OpenDoor();
    // set time of the last door open
    LastDoorOpenTime = currentSeconds;
  }
  if ( currentSeconds - LastDoorOpenTime > DoorCloseDelay ) {
    CloseDoor();
  }
 
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
  float TotalMass = 0.f;
  // Find all the overlapping actors
  TArray<AActor*> OverlappingActors;
  if ( !PressurePlate ) { return TotalMass; }
  PressurePlate->GetOverlappingActors( OUT OverlappingActors );
  // Iterate through hem adding their masses
  for ( const auto& Actor : OverlappingActors) {
    TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
    UE_LOG( LogTemp, Warning, TEXT( "%s on pressure plate" ), *(Actor->GetName()) );
  }
  return TotalMass;
}

