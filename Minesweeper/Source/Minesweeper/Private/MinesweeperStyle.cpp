// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperStyle.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FMinesweeperStyle::StyleInstance = nullptr;

// Initializes the Slate style
void FMinesweeperStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

// Shuts down the Slate style
void FMinesweeperStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

// Returns the name of this style set
FName FMinesweeperStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("MinesweeperStyle"));
	return StyleSetName;
}


const FVector2D Icon90x90(96.0f, 96.0f);
const FVector2D Icon96x96(96.0f, 96.0f);

// Creates the Slate style set and defines its content

TSharedRef<FSlateStyleSet> FMinesweeperStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("MinesweeperStyle"));
	
	// Path to the Minesweeper icon
	FString IconPath = FPaths::Combine(IPluginManager::Get().FindPlugin("MinesweeperTool")->GetBaseDir(), TEXT("Resources/MinesweeperIcon.png"));

	// Define the Minesweeper icon with a brush
	Style->Set("Minesweeper.Icon", new FSlateImageBrush(IconPath, FVector2D(96.0f, 96.0f)));
	

	return Style;
}


void FMinesweeperStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FMinesweeperStyle::Get()
{
	return *StyleInstance;

	
}
