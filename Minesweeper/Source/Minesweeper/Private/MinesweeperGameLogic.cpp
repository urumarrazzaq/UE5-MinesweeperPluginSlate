// Fill out your copyright notice in the Description page of Project Settings.


#include "MinesweeperGameLogic.h"
#include "Containers/Queue.h"

// Starts a new game with the given dimensions, number of bombs, and optional random seed
void FMinesweeperGameLogic::NewGame(int32 InW, int32 InH, int32 InBombs, int32 RandomSeed)
{
    Width = FMath::Max(1, InW);
    Height = FMath::Max(1, InH);
    const int32 MaxBombs = FMath::Clamp(InBombs, 0, Width * Height - 1);

    Grid.Empty();
    Grid.SetNum(Width * Height);
    bGameOver = false;

    FRandomStream Rng(RandomSeed == 0 ? FPlatformTime::Cycles() : RandomSeed);
    PlaceBombs(MaxBombs, Rng);
    ComputeAdjacency();
}

// Checks whether the given coordinates are inside the grid bounds
bool FMinesweeperGameLogic::IsValid(int32 X, int32 Y) const
{
    return (X >= 0 && X < Width && Y >= 0 && Y < Height);
}

// Randomly places bombs on the grid using the provided RNG
void FMinesweeperGameLogic::PlaceBombs(int32 Bombs, FRandomStream& Rng)
{
    TArray<int32> Indices;
    Indices.Reserve(Width * Height);
    for (int32 i = 0; i < Width * Height; ++i) Indices.Add(i);
    for (int32 i = 0; i < Bombs; ++i)
    {
        const int32 Pick = Rng.RandRange(i, Indices.Num() - 1);
        Indices.Swap(i, Pick);
        Grid[Indices[i]].bIsBomb = true;
    }
}

// Computes the number of adjacent bombs for each tile
void FMinesweeperGameLogic::ComputeAdjacency()
{
    for (int32 y = 0; y < Height; ++y)
    {
        for (int32 x = 0; x < Width; ++x)
        {
            FMSPTile& T = Grid[y * Width + x];
            if (!T.bIsBomb)
            {
                T.Adjacent = CountAdj(x, y);
            }
        }
    }
}

// Counts bombs around a specific tile at (X, Y)
int32 FMinesweeperGameLogic::CountAdj(int32 X, int32 Y) const
{
    int32 Count = 0;
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            if (dx == 0 && dy == 0) continue;
            const int32 nx = X + dx, ny = Y + dy;
            if (IsValid(nx, ny) && Grid[ny * Width + nx].bIsBomb)
                ++Count;
        }
    }
    return Count;
}

// Handles a tile click, updating game state
//and reporting whether a bomb was hit or tiles changed
void FMinesweeperGameLogic::Click(int32 X, int32 Y, bool& bOutHitBomb, bool& bOutChanged)
{
    bOutHitBomb = false;
    bOutChanged = false;
    if (!IsValid(X, Y) || bGameOver) return;

    FMSPTile& T = Grid[Y * Width + X];
    if (T.bRevealed) return;

    if (T.bIsBomb)
    {
        bOutHitBomb = true;
        bGameOver = true;
        // Optionally reveal all bombs (not required but useful feedback):
        for (FMSPTile& R : Grid) if (R.bIsBomb) R.bRevealed = true;
        bOutChanged = true;
        return;
    }

    Reveal(X, Y);
    if (T.Adjacent == 0) FloodFillZeros(X, Y);
    bOutChanged = true;
}

// Marks a specific tile as revealed
void FMinesweeperGameLogic::Reveal(int32 X, int32 Y)
{
    if (!IsValid(X, Y)) return;
    FMSPTile& T = Grid[Y * Width + X];
    T.bRevealed = true;
}

// Reveals all connected tiles with zero adjacent bombs using a flood-fill algorithm
void FMinesweeperGameLogic::FloodFillZeros(int32 StartX, int32 StartY)
{
    TQueue<FIntPoint> Q;
    Q.Enqueue(FIntPoint(StartX, StartY));

    while (!Q.IsEmpty())
    {
        FIntPoint P;
        Q.Dequeue(P);
        for (int dy = -1; dy <= 1; ++dy)
        {
            for (int dx = -1; dx <= 1; ++dx)
            {
                if (dx == 0 && dy == 0) continue;
                const int32 nx = P.X + dx, ny = P.Y + dy;
                if (!IsValid(nx, ny)) continue;

                FMSPTile& N = Grid[ny * Width + nx];
                if (!N.bRevealed && !N.bIsBomb)
                {
                    N.bRevealed = true;
                    if (N.Adjacent == 0)
                    {
                        Q.Enqueue(FIntPoint(nx, ny));
                    }
                }
            }
        }
    }
}