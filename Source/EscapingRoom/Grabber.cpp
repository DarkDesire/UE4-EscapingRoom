// Copyright Eldar Nezametdinov 2016

#include "EscapingRoom.h"
#include "Grabber.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

  FString ObjectName = GetOwner()->GetName();
  UE_LOG( LogTemp, Warning, TEXT( "Grabber is working in %s" ), *ObjectName );
  PlayerController = GetWorld()->GetFirstPlayerController();
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

  // Get player view point this tick
  FVector PlayerViewPointLocation;
  FRotator PlayerViewPointRotation;

  PlayerController->GetPlayerViewPoint(
    OUT PlayerViewPointLocation,
    OUT PlayerViewPointRotation );

  FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

  // Drawing a red debug line
  DrawDebugLine(
    GetWorld(),
    PlayerViewPointLocation,
    LineTraceEnd,
    FColor( 255, 0, 0 ),
    false,
    0.f,
    0,
    10.f
  );


  // Loging out for test
   UE_LOG( LogTemp, Warning, TEXT( " Location is: %s , rotation: %s" ), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString() );

  // Ray-cast out to reach distance 



}

