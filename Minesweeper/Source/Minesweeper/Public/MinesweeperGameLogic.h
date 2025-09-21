// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FMSPTile
{
	bool bIsBomb = false;
	bool bRevealed = false;
	int32 Adjacent = 0;
};

class FMinesweeperGameLogic
{
public:
	// Starts a new game with given width, height, bomb count, and optional random seed
	void NewGame(int32 InW, int32 InH, int32 InBombs, int32 RandomSeed = 0);
	
	bool IsValid(int32 X, int32 Y) const;

	// Returns whether the game is over
	bool IsGameOver() const { return bGameOver; }

	// Returns: true if click was processed; out flags for what happened.
	void Click(int32 X, int32 Y, bool& bOutHitBomb, bool& bOutChanged);
	
	// Accessors
	const FMSPTile& Get(int32 X, int32 Y) const { return Grid[Y * Width + X]; }
	int32 GetWidth() const { return Width; }
	int32 GetHeight() const { return Height; }

private:

	// Place bombs randomly on the grid
	void PlaceBombs(int32 Bombs, FRandomStream& Rng);

	// Computes the number of adjacent bombs for each tile
	void ComputeAdjacency();

	// Counts bombs around a specific tile
	int32 CountAdj(int32 X, int32 Y) const;

	// Reveal a tile
	void Reveal(int32 X, int32 Y);

	// Recursively reveals zero-adjacent tiles
	void FloodFillZeros(int32 StartX, int32 StartY);

private:
	TArray<FMSPTile> Grid;
	int32 Width = 0;
	int32 Height = 0;
	bool bGameOver = false;
};