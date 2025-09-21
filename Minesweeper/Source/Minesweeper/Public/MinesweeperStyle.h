// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Styling/SlateStyle.h"

class FMinesweeperStyle
{
public:

	static void Initialize();

	static void Shutdown();

	// reloads textures used by slate renderer
	static void ReloadTextures();

	//return The Slate style set for the Shooter game
	static const ISlateStyle& Get();

	static FName GetStyleSetName();

private:

	// Creates the actual style set
	static TSharedRef< class FSlateStyleSet > Create();

private:

	// Holds the instance of the style set
	static TSharedPtr< class FSlateStyleSet > StyleInstance;
};