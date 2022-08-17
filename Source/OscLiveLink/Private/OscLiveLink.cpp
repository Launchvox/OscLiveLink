// ©2021-2022, Launchvox, Inc. All rights reserved.

#include "OscLiveLink.h"
#include "OscTrackingLiveLink.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "OSCLiveLink_Settings.h"


#define LOCTEXT_NAMESPACE "FOscLiveLinkModule"

DEFINE_LOG_CATEGORY(LogOscLiveLink);

// This code will execute after the module is loaded into memory; 
//the exact timing is specified in the .uplugin file per-module
void FOscLiveLinkModule::StartupModule()
{

	PoseTrackingLiveLinkInstance = MakeShared<FOscTrackingLiveLink, ESPMode::ThreadSafe>();
	
	// Register Engine callbacks
	FCoreDelegates::OnFEngineLoopInitComplete.AddRaw(this, &FOscLiveLinkModule::OnEngineLoopInitComplete);
	
}

// This function is called during shutdown to clean up the module.  
// To support dynamic reloading, the engine will call this function before unloading the module.
void FOscLiveLinkModule::ShutdownModule()
{
	PoseTrackingLiveLinkInstance->Shutdown();
	PoseTrackingLiveLinkInstance = nullptr;

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "Live Link OSC");
	}

}

UOSCServer* FOscLiveLinkModule::GetOSCServer() const
{
	return OSCServer.Get();
}

void FOscLiveLinkModule::OnEngineLoopInitComplete()
{
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Plugins", "Live Link OSC",
			LOCTEXT("OSCLiveLinkPluginSettingsName", "Live Link OSC"),
			LOCTEXT("OSCLiveLinkPluginSettingsDescription", "Configure the OSC Live Link plug-in."),
			GetMutableDefault<UOSCLiveLink_Settings>()
		);
	}
	UOSCLiveLink_Settings* Plugin_Settings = GetMutableDefault<UOSCLiveLink_Settings>();
	InitializeOSCServer(Plugin_Settings->ListeningAddress, Plugin_Settings->ListeningPort);
	PoseTrackingLiveLinkInstance->Init();
}

void FOscLiveLinkModule::InitializeOSCServer(FString Address, int Port)
{
	if (OSCServer)
	{
		OSCServer->Stop();
	}

	if (OSCServer)
	{


		OSCServer->SetAddress(Address, Port);
		OSCServer->Listen();
	}
	else
	{
		OSCServer.Reset(UOSCManager::CreateOSCServer(Address, Port, false, true, FString("FOscLiveLinkServer"), GetTransientPackage()));

#if WITH_EDITOR
		// Allow it to tick in editor, so that messages are parsed.
		if (OSCServer)
		{
			OSCServer->SetTickInEditor(true);
		}
#endif // WITH_EDITOR
	}

}

IMPLEMENT_MODULE(FOscLiveLinkModule, OscLiveLink)

#undef LOCTEXT_NAMESPACE