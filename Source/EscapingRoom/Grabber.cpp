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

  FString OwnerName = GetOwner()->GetName();

  UE_LOG( LogTemp, Warning, TEXT( "%s . Grabber starts work." ), *OwnerName );
  PlayerController = GetWorld()->GetFirstPlayerController();

  /// Look for attached Physics Handle
  PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

  if ( PhysicsHandle ) {
    UE_LOG( LogTemp, Warning, TEXT( "PhysicsHandleComponent found" ) );
  }
  else 
  {
    UE_LOG( LogTemp, Error, TEXT( "%s missing a PhysicsHandleComponent" ), *OwnerName);
  }


  /// Look for attached Input Component (only appers at run time)
  InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

  if ( InputComponent ) {
    UE_LOG( LogTemp, Warning, TEXT( "InputComponent found" ) );
    // Bind the input actions
    InputComponent->BindAction(
      "Grab", // action map
      IE_Pressed, 
      this,
      &UGrabber::Grab
    );
  }
  else {
    UE_LOG( LogTemp, Error, TEXT( "%s missing a InputComponent" ), *OwnerName);
  }
}

void UGrabber::Grab() {
  UE_LOG( LogTemp, Warning, TEXT( "Grab key pressed" ) );
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

  /// Get player view point this tick
  FVector PlayerViewPointLocation;
  FRotator PlayerViewPointRotation;

  PlayerController->GetPlayerViewPoint(
    OUT PlayerViewPointLocation,
    OUT PlayerViewPointRotation );

  FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

  /// Drawing a red debug line
  DrawDebugLine(
    GetWorld(),
    PlayerViewPointLocation,
    LineTraceEnd,
    FColor( 255, 0, 0 ),
    false,
    0.f,
    0,
    2.f
  );

  /// Setup query parameters
  FCollisionQueryParams TraceParameters( 
    FName( TEXT( "" )),  // don't care about tag
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
   AActor* HitActor = Hit.GetActor();
   if ( HitActor ) {
     FString HitActorName = HitActor->GetName();
     UE_LOG( LogTemp, Warning, TEXT( "Line-trace hit a: %s" ), *HitActorName );
   }

}

