

#include "Items/Items.h"
#include "Slash/DebugMacros.h"

// #define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 15.f, 20, FColor::Red, true); 
// We can define it in DebugMacros.h so we can use it anywhere where DebugMacros.h is included
// If I define something (called Macro) before compiling the program will replace it with the actual line of code so semicolon is not required on call 
// Example DRAW_SPHERE(Location)

AItems::AItems()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
}

void AItems::BeginPlay()
{
	Super::BeginPlay();
	
}
float AItems::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}
float AItems::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}
void AItems::RotateObject()
{
	SetActorRotation(FRotator(0.f, 30.f, 0.f) * RunningTime);
}

void AItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

}

