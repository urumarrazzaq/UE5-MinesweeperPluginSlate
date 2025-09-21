// Fill out your copyright notice in the Description page of Project Settings.

#include "Slate/SMinesweeperWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Misc/MessageDialog.h"
#include "GenericPlatform/GenericApplication.h" // FDisplayMetrics

#define LOCTEXT_NAMESPACE "SMinesweeperWidget"

// Visual constants to match the current look
static constexpr float kTilePx  = 24.f; // per-tile square content
static constexpr float kSlotPad = 1.f;  // grid slot padding

void SMinesweeperWidget::Construct(const FArguments& InArgs)
{
    Game.NewGame(GridW, GridH, Bombs);

    ChildSlot
    [
        SNew(SVerticalBox)

        + SVerticalBox::Slot().AutoHeight().Padding(8.f, 8.f)
        [
            BuildHeaderBar()
        ]

        + SVerticalBox::Slot().FillHeight(1.f).Padding(8.f, 4.f)
        [
            SNew(SScrollBox)
            + SScrollBox::Slot()
            [
                BuildBoard()
            ]
        ]
    ];

    // Initialize warning once
    UpdateBombWarning();
}

//Top Bar controls for inputting height, width, bombs
TSharedRef<SWidget> SMinesweeperWidget::BuildHeaderBar()
{
    return
    SNew(SHorizontalBox)

    // Width
    + SHorizontalBox::Slot().AutoWidth().Padding(0,0,8,0)
    [
        SNew(STextBlock).Text(LOCTEXT("WidthLbl", "Width"))
    ]
    + SHorizontalBox::Slot().AutoWidth().Padding(0,0,16,0)
    [
        SNew(SSpinBox<int32>)
        .MinValue(1).MaxValue(200)
        .Value_Lambda([this]{ return GridW; })
        .OnValueChanged_Lambda([this](int32 V)
        {
            GridW = V;
            UpdateBombWarning(); // keep warning live
        })
        .Delta(1)
    ]

    // Height
    + SHorizontalBox::Slot().AutoWidth().Padding(0,0,8,0)
    [
        SNew(STextBlock).Text(LOCTEXT("HeightLbl", "Height"))
    ]
    + SHorizontalBox::Slot().AutoWidth().Padding(0,0,16,0)
    [
        SNew(SSpinBox<int32>)
        .MinValue(1).MaxValue(200)
        .Value_Lambda([this]{ return GridH; })
        .OnValueChanged_Lambda([this](int32 V)
        {
            GridH = V;
            UpdateBombWarning(); // keep warning live
        })
        .Delta(1)
    ]

    // Bombs
    + SHorizontalBox::Slot().AutoWidth().Padding(0,0,8,0)
    [
        SNew(STextBlock).Text(LOCTEXT("BombsLbl", "Bombs"))
    ]
    + SHorizontalBox::Slot().AutoWidth().Padding(0,0,16,0)
    [
        SNew(SSpinBox<int32>)
        .MinValue(0).MaxValue(40000)
        .Value_Lambda([this]{ return Bombs; })
        .OnValueChanged_Lambda([this](int32 V)
        {
            Bombs = V;
            UpdateBombWarning(); // keep warning live
        })
        .Delta(1)
    ]

    // Start + warning
    + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
    [
        SNew(SHorizontalBox)

        + SHorizontalBox::Slot().AutoWidth()
        [
            SNew(SButton)
            .Text(LOCTEXT("NewGame", "Start New Game"))
            .OnClicked(this, &SMinesweeperWidget::OnNewGameClicked)
        ]

        + SHorizontalBox::Slot().AutoWidth().Padding(8,0,0,0).VAlign(VAlign_Center)
        [
            SAssignNew(BombWarningText, STextBlock)
            .Text(FText::GetEmpty())
            .ColorAndOpacity(FSlateColor(FLinearColor::Yellow))
            .Visibility(EVisibility::Collapsed)
        ]
    ];
}

//Creating Minesweeper Grid
TSharedRef<SWidget> SMinesweeperWidget::BuildBoard()
{
    SAssignNew(GridPanel, SGridPanel);

    const int32 W = Game.GetWidth();
    const int32 H = Game.GetHeight();

    static FButtonStyle Flat = []{
        FButtonStyle S = FAppStyle::Get().GetWidgetStyle<FButtonStyle>("Button");
        S.NormalPadding  = FMargin(0);
        S.PressedPadding = FMargin(0);
        return S;
    }();

    for (int32 y = 0; y < H; ++y)
    {
        for (int32 x = 0; x < W; ++x)
        {
            const FMSPTile& T = Game.Get(x, y);
            const FText Label = T.bRevealed
                ? (T.bIsBomb ? LOCTEXT("Bomb", "💣") : FText::AsNumber(T.Adjacent))
                : FText();

            GridPanel->AddSlot(x, y)
            .Padding(kSlotPad)
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            [
                SNew(SButton)
                .ButtonStyle(&Flat)
                .ContentPadding(FMargin(0))
                .OnClicked_Lambda([this, x, y]() { return OnTileClicked(x, y); })
                [
                    SNew(SScaleBox)
                    .Stretch(EStretch::ScaleToFit)
                    .HAlign(HAlign_Center)
                    .VAlign(VAlign_Center)
                    [
                        SNew(SBox)
                        .MinDesiredWidth(kTilePx)
                        .MinDesiredHeight(kTilePx)
                        [
                            SNew(STextBlock)
                            .Justification(ETextJustify::Center)
                            .Text(Label)
                        ]
                    ]
                ]
            ];
        }
    }

    return GridPanel.ToSharedRef();
}

//Runs Start New Game
FReply SMinesweeperWidget::OnNewGameClicked()
{
    const int32 TotalTiles = GridW * GridH;

    //Screen-width-based column limit (DPI-aware)
    FDisplayMetrics DM;
    FSlateApplication::Get().GetInitialDisplayMetrics(DM);

    const int32 ScreenWidthPx = DM.PrimaryDisplayWorkAreaRect.Right - DM.PrimaryDisplayWorkAreaRect.Left;

    // Convert screen pixels to Slate units
    float AppScale = FSlateApplication::Get().GetApplicationScale();
    if (AppScale <= 0.f) AppScale = 1.f;

    const float ScreenWidthSlate = ScreenWidthPx / AppScale;

    // Reserve some space for left/right padding + potential scrollbar, etc.
    const float ReservedLR = 32.f;

    // Effective width we can draw tiles into (in Slate units)
    const float UsableWidth = FMath::Max(0.f, ScreenWidthSlate - ReservedLR);

    // Each column costs: tile size + left+right slot padding
    const float PerColumnSlate = kTilePx + (kSlotPad * 2.f);

    const int32 MaxColsByScreen = FMath::Max(1, FMath::FloorToInt(UsableWidth / PerColumnSlate));

    if (GridW > MaxColsByScreen)
    {
        const FText Msg = FText::Format(
            LOCTEXT("TooManyColsForScreen",
                "Width ({0}) exceeds the screen's horizontal capacity ({1} columns at current resolution).\n"
                "Reduce Width or increase screen resolution."),
            FText::AsNumber(GridW),
            FText::AsNumber(MaxColsByScreen)
        );
        FMessageDialog::Open(EAppMsgType::Ok, Msg);
        return FReply::Handled();
    }

    // Hard rule: block if bombs >= 50% of tiles
    if (TotalTiles > 0 && Bombs >= TotalTiles * 0.5f)
    {
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("TooManyBombs", "Bombs cannot be 50% or more of the grid!"));
        return FReply::Handled();
    }

    // Clamp bombs to feasible amount (ensure at least one safe tile)
    const int32 MaxBombs = FMath::Clamp(Bombs, 0, FMath::Max(0, TotalTiles - 1));
    Bombs = MaxBombs;

    Game.NewGame(GridW, GridH, Bombs);
    RebuildGrid();

    // Re-check soft warning after starting
    UpdateBombWarning();

    return FReply::Handled();
}

// Rebuilds Game whenever new game is started
void SMinesweeperWidget::RebuildGrid()
{
    if (!GridPanel.IsValid()) return;

    GridPanel->ClearChildren();

    const int32 W = Game.GetWidth();
    const int32 H = Game.GetHeight();

    static FButtonStyle Flat = []{
        FButtonStyle S = FAppStyle::Get().GetWidgetStyle<FButtonStyle>("Button");
        S.NormalPadding  = FMargin(0);
        S.PressedPadding = FMargin(0);
        return S;
    }();

    for (int32 y = 0; y < H; ++y)
    {
        for (int32 x = 0; x < W; ++x)
        {
            const FMSPTile& T = Game.Get(x, y);
            const FText Label = T.bRevealed
                ? (T.bIsBomb ? LOCTEXT("Bomb", "💣") : FText::AsNumber(T.Adjacent))
                : FText();

            GridPanel->AddSlot(x, y)
            .Padding(kSlotPad)
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            [
                SNew(SButton)
                .ButtonStyle(&Flat)
                .ContentPadding(FMargin(0))
                .OnClicked_Lambda([this, x, y]() { return OnTileClicked(x, y); })
                [
                    SNew(SScaleBox)
                    .Stretch(EStretch::ScaleToFit)
                    .HAlign(HAlign_Center)
                    .VAlign(VAlign_Center)
                    [
                        SNew(SBox)
                        .MinDesiredWidth(kTilePx)
                        .MinDesiredHeight(kTilePx)
                        [
                            SNew(STextBlock)
                            .Justification(ETextJustify::Center)
                            .Text(Label)
                        ]
                    ]
                ]
            ];
        }
    }
}

//Processes Tile Click
FReply SMinesweeperWidget::OnTileClicked(int32 X, int32 Y)
{
    bool bHitBomb = false, bChanged = false;
    Game.Click(X, Y, bHitBomb, bChanged);
    if (bChanged)
    {
        RebuildGrid();
    }
    if (bHitBomb)
    {
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("GameOver", "Game Over!"));
    }
    return FReply::Handled();
}

// Centralized: show/hide yellow warning for >20% bombs
void SMinesweeperWidget::UpdateBombWarning()
{
    if (!BombWarningText.IsValid()) return;

    const int32 TotalTiles = GridW * GridH;
    const bool bWarn = (TotalTiles > 0) && (Bombs > TotalTiles * 0.2f);

    if (bWarn)
    {
        BombWarningText->SetText(LOCTEXT("BombWarning", "⚠ High bomb ratio (>20%)"));
        BombWarningText->SetVisibility(EVisibility::Visible);
    }
    else
    {
        BombWarningText->SetText(FText::GetEmpty());
        BombWarningText->SetVisibility(EVisibility::Collapsed);
    }
}

#undef LOCTEXT_NAMESPACE
