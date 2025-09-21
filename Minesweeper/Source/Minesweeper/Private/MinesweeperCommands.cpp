// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperCommands.h"

#define LOCTEXT_NAMESPACE "FMinesweeperModule"

void FMinesweeperCommands::RegisterCommands()
{
	UI_COMMAND(OpenTool, "Minesweeper", "Open the minesweeper tool window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
