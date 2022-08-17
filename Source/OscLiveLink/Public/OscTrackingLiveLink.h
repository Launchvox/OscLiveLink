// ©2021-2022, Launchvox, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LiveLinkProvider.h"
#include "Roles/LiveLinkBasicRole.h"
#include "OSCManager.h"
#include "OSCMessage.h"
#include "OSCBundle.h"
#include "OSCServer.h"
#include "OSCLiveLink_Settings.h"



/**
 * 
 */

class OSCLIVELINK_API FOscTrackingLiveLink : public IModularFeature, public TSharedFromThis<FOscTrackingLiveLink>
{
public:
	FOscTrackingLiveLink();
	virtual ~FOscTrackingLiveLink();

	void Init();
	void Shutdown();

	static FName GetModularFeatureName()
	{
		static FName FeatureName = FName(TEXT("OscPoseTrackingLiveLink"));
		return FeatureName;
	}

protected:
	static void OnConnectionStatusChanged();

private:

	TSharedPtr<ILiveLinkProvider> LiveLinkProvider;
	FDelegateHandle ConnectionStatusChangedHandle;
	TSharedPtr<FOscTrackingLiveLink> OscLiveLinkComponent;
	TArray<FName> Subjects;
	TArray<FName> ShapeNames;
	FDelegateHandle OscHandle;
	TArray<float> Blendshapes;
	EOscRotationFormat RotationFormat;

	void InitializeSubject();
	void UpdateSubject();
	void ClearLiveLinkProvider();

	void GetFloatWithFallbackInt(const FOSCMessage& Message, float& buffer, uint16 index);

	/** Receive OSC server message handler */
	void OSCReceivedMessageEvent(const FOSCMessage& Message, const FString& IPAddress, uint16 Port);

	void OSCReceivedMessageBundleEvent(const FOSCBundle& Bundle, const FString& IPAddress, uint16 Port);

	FName PoseTrackingLiveLinkSubjectName;
	// The id of this device 
	FName LocalDeviceId;
};

