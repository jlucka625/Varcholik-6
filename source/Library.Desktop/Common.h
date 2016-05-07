/** @file Common.h
*   @brief Common values used by both platform projects
*
*   @author Alex Stopar (amstopar)
*/
#pragma once


#include "pch.h"

///Default width of the window
const int WINDOW_SCREEN_WIDTH = 800;

///Default height of the window
const int WINDOW_SCREEN_HEIGHT = 600;

///Name of application
const std::string APP_NAME = "FIEAGameEngine";

///This struct holds common colors for both platforms
struct Color
{
	///The red part of the color value
	float red;

	///The blue part of the color value
	float blue;

	///The green part of the color value
	float green;

	///The alpha part of the color value
	float alpha;
};

///Color of cleared screen
const Color SCREEN_COLOR = { 0.7647f, 0.153f, 0.1686f, 1.0f };