// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MinesweeperStyle.h"
#include "EditorStyleSet.h" // For icon usage

class FMinesweeperCommands : public TCommands<FMinesweeperCommands>
{
public:

	//Defining Minesweeper command context and style
	FMinesweeperCommands()
		: TCommands<FMinesweeperCommands>(TEXT("Minesweeper"), NSLOCTEXT("Minesweeper", "ContextDesc", "Minesweeper Tool"), NAME_None, FMinesweeperStyle::GetStyleSetName())
	{
	}

	// Registers the plugin's commands
	virtual void RegisterCommands() override;

	//Command for Minesweeper tool
	TSharedPtr<FUICommandInfo> OpenTool;
};
