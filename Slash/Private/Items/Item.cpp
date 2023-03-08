

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

	/* UWorld* World = GetWorld();
	if (World)
	{
		FVector Location = GetActorLocation();
		DrawDebugSphere(World, Location, 15.f, THIRTY, FColor::Red, false, 30.f);
	} */

	FVector Location = GetActorLocation();
	FVector Forward = GetActorForwardVector();
	// We don't need to use semicolon but because Visual Studio things we are not finished with the line it indents it and it can be annoying
	DRAW_SPHERE_CUSTOM_COLOR(Location, FColor::Blue);
	DRAW_VECTOR(Location, Location + Forward * 100.f);
	FRotator Rotation = GetActorRotation();
	DRAW_CROSSHAIRS(Location, Rotation);

	
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* UE_LOG(LogTemp, Warning, TEXT("DeltaTime: %f"), DeltaTime);

	if (GEngine)
	{
		FString Name = GetName();
		FString Message = FString::Printf(TEXT("Item Name: %s"), *Name);
		GEngine->AddOnScreenDebugMessage(2, 60.f, FColor::Cyan, Message);

		UE_LOG(LogTemp, Warning, TEXT("Item Name: %s"), *Name);
	} */


}

