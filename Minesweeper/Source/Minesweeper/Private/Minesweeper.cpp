// Copyright Epic Games, Inc. All Rights Reserved.

#include "Minesweeper.h"
#include "MinesweeperCommands.h"
#include "Widgets/Docking/SDockTab.h"
#include "Framework/Docking/TabManager.h" // ← Add this for tab manager
#include "ToolMenus.h"
#include "LevelEditor.h"
#include "Slate/SMinesweeperWidget.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Styling/SlateStyle.h"

static const FName MinesweeperTabName("Minesweeper");

#define LOCTEXT_NAMESPACE "FMinesweeperModule"

void FMinesweeperModule::StartupModule()
{
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
    FMinesweeperStyle::Initialize();
    FMinesweeperStyle::ReloadTextures();

    FMinesweeperCommands::Register();
    
    PluginCommands = MakeShareable(new FUICommandList);

    PluginCommands->MapAction(
        FMinesweeperCommands::Get().OpenTool,
        FExecuteAction::CreateRaw(this, &FMinesweeperModule::PluginButtonClicked),
        FCanExecuteAction());

    // Register the plugin button in the toolbar and the menu
    UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMinesweeperModule::RegisterMenus));

    // Register the Nomad Tab (Minesweeper tool window)
    FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MinesweeperTabName,
        FOnSpawnTab::CreateRaw(this, &FMinesweeperModule::SpawnMinesweeperTab))
        .SetDisplayName(LOCTEXT("MinesweeperTabTitle", "Minesweeper Tool"))
        .SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMinesweeperModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
    
    UToolMenus::UnRegisterStartupCallback(this);
    UToolMenus::UnregisterOwner(this);

    FMinesweeperStyle::Shutdown();
    FMinesweeperCommands::Unregister();
    
    // Unregister the tab spawner
    FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MinesweeperTabName);
}

void FMinesweeperModule::PluginButtonClicked()
{
    // Prepare dialog text
    FText DialogText = LOCTEXT("PluginButtonDialogText", "Welcome to Minesweeper! Press OK to play or Cancel to quit.");

    // Show the dialog and capture the result
    EAppReturnType::Type Result = FMessageDialog::Open(
        EAppMsgType::OkCancel, // Ok + Cancel buttons
        DialogText
    );

    // If pressed OK, open the Minesweeper tool tab
    if (Result == EAppReturnType::Ok)
    {
        FGlobalTabmanager::Get()->TryInvokeTab(MinesweeperTabName);
    }
}

void FMinesweeperModule::RegisterMenus()
{
    // Extend the Level Editor Window menu
    UToolMenu* WindowMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");

    // Add Minesweeper section to the Window menu
    FToolMenuSection& WindowSection = WindowMenu->FindOrAddSection("WindowLayout");
    WindowSection.AddMenuEntryWithCommandList(FMinesweeperCommands::Get().OpenTool, PluginCommands);

    // Extend the Level Editor Toolbar
    UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");

    // Add a section in the toolbar for the Minesweeper tool button
    FToolMenuSection& ToolbarSection = ToolbarMenu->FindOrAddSection("PluginTools");

    // Creating a button for the toolbar with an icon
    FToolMenuEntry& Entry = ToolbarSection.AddEntry(FToolMenuEntry::InitToolBarButton(FMinesweeperCommands::Get().OpenTool));

    // Setting the command list for the button (this ties the button to the OpenTool command)
    Entry.SetCommandList(PluginCommands);

    // Icon for the button
    FSlateIcon Icon = FSlateIcon(FName(TEXT("MinesweeperStyle")), TEXT("Minesweeper.Icon"));
    Entry.Icon = TAttribute<FSlateIcon>(Icon);
}


// Spawn the Minesweeper tool tab in the editor
TSharedRef<SDockTab> FMinesweeperModule::SpawnMinesweeperTab(const FSpawnTabArgs& Args)
{
    return SNew(SDockTab).TabRole(ETabRole::NomadTab)
    [
        SNew(SMinesweeperWidget) // Slate tool content
    ];
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMinesweeperModule, Minesweeper)
