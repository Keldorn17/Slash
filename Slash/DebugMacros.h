#pragma once
#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 15.f, 20, FColor::Red, true);
#define DRAW_LINE(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f);
#define DRAW_POINT(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, true);
#define DRAW_VECTOR(StartLocation, EndLocation) if (GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f); \
		DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Red, true); \
	}
// If we use back slash we can tell to the compiler that we use multiple line of definition 
// It is important that after back slash even white charachers are not allowed
#define DRAW_SPHERE_CUSTOM_COLOR(Location, Color) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 15.f, 20, Color, true);
#define DRAW_CROSSHAIRS(Location, Rotation) if (GetWorld()) DrawDebugCrosshairs(GetWorld(), Location, Rotation, 10, FColor::Green, true);
