// Copyright Epic Games, Inc. All Rights Reserved.

#include "OscLiveLink.h"
#include "OscTrackingLiveLink.h"

#define LOCTEXT_NAMESPACE "FOscLiveLinkModule"

DEFINE_LOG_CATEGORY(LogOscLiveLink);


//TSharedPtr<FOscTrackingLiveLink, ESPMode::ThreadSafe> PoseTrackingLiveLinkInstance;

/*void FOscLiveLinkModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	PoseTrackingLiveLinkInstance = MakeShared<FOscTrackingLiveLink, ESPMode::ThreadSafe>();
	PoseTrackingLiveLinkInstance->Init();

}
void FOscLiveLinkModule::ShutdownModule()
{
	PoseTrackingLiveLinkInstance->Shutdown();
	PoseTrackingLiveLinkInstance = nullptr;
}
*/

////////////////////////////////
///////////////////////////////

void FOscLiveLinkModule::StartupModule()
{
	
	PoseTrackingLiveLinkInstance = MakeShared<FOscTrackingLiveLink, ESPMode::ThreadSafe>();
	
	// Register Engine callbacks
	FCoreDelegates::OnFEngineLoopInitComplete.AddRaw(this, &FOscLiveLinkModule::OnEngineLoopInitComplete);
	
}

void FOscLiveLinkModule::ShutdownModule()
{
	PoseTrackingLiveLinkInstance->Shutdown();
	PoseTrackingLiveLinkInstance = nullptr;
}

UOSCServer* FOscLiveLinkModule::GetOSCServer() const
{
	return OSCServer.Get();
}

void FOscLiveLinkModule::OnEngineLoopInitComplete()
{
	InitializeOSCServer();
	PoseTrackingLiveLinkInstance->Init();
}

void FOscLiveLinkModule::InitializeOSCServer()
{
	if (OSCServer)
	{
		OSCServer->Stop();
	}

	//Auto determines local IP.
	const FString& ServerAddress = "";
	uint16 ServerPort = 9000;

	if (OSCServer)
	{
		OSCServer->SetAddress(ServerAddress, ServerPort);
		OSCServer->Listen();
	}
	else
	{
		OSCServer.Reset(UOSCManager::CreateOSCServer(ServerAddress, ServerPort, false, true, FString("FOscLiveLinkServer"), GetTransientPackage()));

#if WITH_EDITOR
		// Allow it to tick in editor, so that messages are parsed.
		if (OSCServer)
		{
			OSCServer->SetTickInEditor(true);
		}
#endif // WITH_EDITOR
	}

}

///////////////////////////////
//////////////////////////////


IMPLEMENT_MODULE(FOscLiveLinkModule, OscLiveLink)

#undef LOCTEXT_NAMESPACE
	

