// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FMinesweeperModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	void RegisterMenus();
	
private:
	
	void OpenToolTab();
	//TSharedRef<class SDockTab> SpawnMinesweeperTab(const class ESpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnMinesweeperTab(const FSpawnTabArgs& Args);
	TSharedPtr<class FUICommandList> PluginCommands;
	void PluginButtonClicked();
};
