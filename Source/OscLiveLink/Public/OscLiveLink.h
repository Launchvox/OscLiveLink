// ©2021-2022, Launchvox, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "UObject/StrongObjectPtr.h"
#include "OscTrackingLiveLink.h"

DECLARE_LOG_CATEGORY_EXTERN(LogOscLiveLink, Log, Log);

class UOSCServer;

class FOscLiveLinkModule : public IModuleInterface
{
public:

	//Global Settings
	const FString& ServerAddress = "127.0.0.1";
	uint16 ServerPort = 9001;
	const static bool UseQuaternionRotation = false;

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	UOSCServer* GetOSCServer() const;

	/** The default OSC server. */
	TStrongObjectPtr<UOSCServer> OSCServer;
	
	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static FOscLiveLinkModule& Get()
	{
		static const FName ModuleName = "OscLiveLink";
		return FModuleManager::LoadModuleChecked<FOscLiveLinkModule>(ModuleName);
	};

	TSharedPtr<FOscTrackingLiveLink, ESPMode::ThreadSafe> PoseTrackingLiveLinkInstance;

private:

	void OnEngineLoopInitComplete();

	/** Start an OSC server and bind a an OSC listener to it. */
	void InitializeOSCServer();

};
