// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "MinesweeperGameLogic.h"

class STextBlock; // + added

class SMinesweeperWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	FReply OnNewGameClicked();
	void RebuildGrid();

	// Click handler per tile
	FReply OnTileClicked(int32 X, int32 Y);

	// Helper function declarations
	TSharedRef<SWidget> BuildHeaderBar();
	TSharedRef<SWidget> BuildBoard();

	// Warning logic
	void UpdateBombWarning();

private:
	// Configure
	int32 GridW = 10;
	int32 GridH = 10;
	int32 Bombs = 10;

	// Logic
	FMinesweeperGameLogic Game;

	// UI references
	TSharedPtr<class SGridPanel> GridPanel;

	// Warning text (yellow) when bombs > 20%
	TSharedPtr<STextBlock> BombWarningText;
};
