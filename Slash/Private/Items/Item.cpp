

#include "Items/Item.h"
#include "Slash/DebugMacros.h"

// #define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 15.f, 20, FColor::Red, true); 
// We can define it in DebugMacros.h so we can use it anywhere where DebugMacros.h is included
// If I define something (called Macro) before compiling the program will replace it with the actual line of code so semicolon is not required on call 
// Example DRAW_SPHERE(Location)

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AItem::BeginPlay()
{
	Super::BeginPlay();

}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}
float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

	// float DeltaZ = Amplitude * FMath::Sin(RunningTime * TimeConstant);

	// AddActorWorldOffset(FVector(0.f, 0.f, DeltaZ));
	
	DRAW_SPHERE_SINGLEFRAME(GetActorLocation());
	DRAW_VECTOR_SINGLEFRAME(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100);

	FVector AvgVector = Avg<FVector>(GetActorLocation(), FVector::ZeroVector);
	DRAW_POINT_SINGLEFRAME(AvgVector);

}

