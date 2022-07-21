// ©2021-2022, Launchvox, Inc. All rights reserved.

#include "OscLiveLink.h"
#include "OscTrackingLiveLink.h"

#define LOCTEXT_NAMESPACE "FOscLiveLinkModule"

DEFINE_LOG_CATEGORY(LogOscLiveLink);

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

IMPLEMENT_MODULE(FOscLiveLinkModule, OscLiveLink)

#undef LOCTEXT_NAMESPACE
	

