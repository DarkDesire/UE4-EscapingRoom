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
  FindPhysicsHandleComponent();
  SetupInputComponent();
}

/// Look for attached Physics Handle Component
void UGrabber::FindPhysicsHandleComponent()
{
  PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

  if ( PhysicsHandle ) {
    UE_LOG( LogTemp, Warning, TEXT( "PhysicsHandleComponent found" ) );
  }
  else {
    UE_LOG( LogTemp, Error, TEXT( "%s missing a PhysicsHandleComponent" ), *(GetOwner()->GetName()) );
  }
}

/// Look for attached Input Component (only appers at run time)
void UGrabber::SetupInputComponent()
{
  InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

  if ( InputComponent ) {
    UE_LOG( LogTemp, Warning, TEXT( "InputComponent found" ) );

    InputComponent->BindAction( "Grab", IE_Pressed, this, &UGrabber::Grab );
    InputComponent->BindAction( "Grab", IE_Released, this, &UGrabber::Release );
  }
  else {
    UE_LOG( LogTemp, Error, TEXT( "%s missing a InputComponent" ), *(GetOwner()->GetName()) );
  }
}

void UGrabber::Grab() {
  UE_LOG( LogTemp, Warning, TEXT( "Grab key pressed" ) );

  // LINE TRACE and see if we hit any actorws with physics body collision channel set
  FHitResult Hit = GetFirstPhysicsBodyInReach();
  auto HitActor = Hit.GetActor();

  if ( HitActor ) { // If we hit something then attach a physics handle
    UE_LOG( LogTemp, Warning, TEXT( "Line-trace hit a: %s" ), *(HitActor->GetName()) );

    auto ComponentToGrab = Hit.GetComponent(); // get the mesh in our case
    // attach physics handle
    if ( !PhysicsHandle ) { return; }
    PhysicsHandle->GrabComponent(
      ComponentToGrab,
      NAME_None, // no bone
      ComponentToGrab->GetOwner()->GetActorLocation(),
      true // allow rotation
    );
  }

}

void UGrabber::Release() {
  UE_LOG( LogTemp, Warning, TEXT( "Grab key released" ) );

  if ( !PhysicsHandle ) { return; }
  PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
  Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

  if ( !PhysicsHandle ) { return; }
  if ( PhysicsHandle->GrabbedComponent ) { // if the physics handle is attached
    PhysicsHandle->SetTargetLocation( GetReachLineEnd() ); // move the object that we're holding
  }
}


const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
  /// Setup query parameters
  FCollisionQueryParams TraceParameters(
    FName( TEXT( "" ) ),  // don't care about tag
    false, // simple trace, not complex
    GetOwner() // ignoring actor *ourself*
  );

  /// Line-trace (AKA Ray-cast) out to reach distance 
  FHitResult HitResult;
  GetWorld()->LineTraceSingleByObjectType(
    OUT HitResult,
    GetReachLineStart(),
    GetReachLineEnd(),
    FCollisionObjectQueryParams( ECollisionChannel::ECC_PhysicsBody ),
    TraceParameters
  );
  return HitResult;
}

FVector UGrabber::GetReachLineStart()
{
  GetPlayerViewPort();
  return  PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
  GetPlayerViewPort();
  return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
}

void UGrabber::GetPlayerViewPort()
{
  GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
    OUT PlayerViewPointLocation,
    OUT PlayerViewPointRotation );
}