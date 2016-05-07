#pragma once

#define PIXEL_TO_METERS_RATIO (1.0f / 30.0f)
#define TIME_STEP  (1.f / 60.f)	//Number of times the world steps through all the physics stuff.
#define VELOCITY_STEP_RATE (10)	//Number of velocity iterations in the world's step.
#define POSITION_STEP_RATE (3)	//Number of position iterations in the world's step.

inline float ConvertPixelToMeter(float value) { return (value * PIXEL_TO_METERS_RATIO); }
inline float ConvertMeterToPixel(float value) { return (value / PIXEL_TO_METERS_RATIO); }

enum BodyTypes
{
	Player = 1,
	Tile = 2,
	Hazard = 4,
	Barrier = 8
};
