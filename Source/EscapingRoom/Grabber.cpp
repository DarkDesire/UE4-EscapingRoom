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
    // Bind the input actions
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
  auto ComponentToGrab = Hit.GetComponent();
  auto HitActor = Hit.GetActor();


  if ( HitActor ) { // If we hit something then attach a physics handle
    UE_LOG( LogTemp, Warning, TEXT( "Line-trace hit a: %s" ), *(HitActor->GetName()) );

    // attach physics handle
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
  PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
  Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

  /// Get player view point this tick
  FVector PlayerViewPointLocation;
  FRotator PlayerViewPointRotation;

  GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
    OUT PlayerViewPointLocation,
    OUT PlayerViewPointRotation );

  FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
 
  if ( PhysicsHandle->GrabbedComponent ) { // if the physics handle is attached
    PhysicsHandle->SetTargetLocation( LineTraceEnd );
    // move the object that we're holding
  }


}


const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{

  /// Get player view point this tick
  FVector PlayerViewPointLocation;
  FRotator PlayerViewPointRotation;

  GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
    OUT PlayerViewPointLocation,
    OUT PlayerViewPointRotation );

  FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

  /// Drawing a red debug line
  // DrawDebugLine( GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor( 255, 0, 0 ), false, 0.f, 0, 2.f );

  /// Setup query parameters
  FCollisionQueryParams TraceParameters(
    FName( TEXT( "" ) ),  // don't care about tag
    false, // simple trace, not complex
    GetOwner() // ignoring actor *ourself*
  );

  /// Lne-trace (AKA Ray-cast) out to reach distance 
  FHitResult Hit;
  GetWorld()->LineTraceSingleByObjectType(
    OUT Hit,
    PlayerViewPointLocation,
    LineTraceEnd,
    FCollisionObjectQueryParams( ECollisionChannel::ECC_PhysicsBody ),
    TraceParameters
  );
  return Hit;
}