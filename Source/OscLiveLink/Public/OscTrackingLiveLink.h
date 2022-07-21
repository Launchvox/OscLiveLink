// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LiveLinkProvider.h"
#include "Roles/LiveLinkBasicRole.h"
#include "OSCManager.h"
#include "OSCMessage.h"
#include "OSCServer.h"


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

	void InitializeSubject();
	void UpdateSubject();
	void ClearLiveLinkProvider();

	/** Receive OSC server message handler */
	void OSCReceivedMessageEvent(const FOSCMessage& Message, const FString& IPAddress, uint16 Port);

	FName PoseTrackingLiveLinkSubjectName;
	// The id of this device 
	FName LocalDeviceId;
};

