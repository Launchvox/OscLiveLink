// Fill out your copyright notice in the Description page of Project Settings.


#include "OscTrackingLiveLink.h"
#include "OscLiveLink.h"
FOscTrackingLiveLink::FOscTrackingLiveLink()
{
	Blendshapes.Init(0.0, 55);
}

FOscTrackingLiveLink::~FOscTrackingLiveLink()
{
	// Should only be called durirng shutdown
	check(IsEngineExitRequested());
}

void FOscTrackingLiveLink::Init()
{
	IModularFeatures::Get().RegisterModularFeature(GetModularFeatureName(), this);
	InitializeSubject();
	OscHandle = FOscLiveLinkModule::Get().OSCServer->OnOscMessageReceivedNative.AddSP(this, &FOscTrackingLiveLink::OSCReceivedMessageEvent);
}

void FOscTrackingLiveLink::Shutdown()
{
	FOscTrackingLiveLink::ClearLiveLinkProvider();
	FOscLiveLinkModule::Get().OSCServer->OnOscMessageReceivedNative.Remove(OscHandle);
	IModularFeatures::Get().UnregisterModularFeature(GetModularFeatureName(), this);
	FCoreDelegates::OnFEngineLoopInitComplete.RemoveAll(this);
}

void FOscTrackingLiveLink::OnConnectionStatusChanged()
{
}

void FOscTrackingLiveLink::InitializeSubject()
{
	if (LiveLinkProvider.IsValid())
	{
		FPlatformMisc::LowLevelOutputDebugString(TEXT("Live Link Provider already started!\n"));
		return;
	}

	LiveLinkProvider = ILiveLinkProvider::CreateLiveLinkProvider(TEXT("HallwayTile OSC"));
	ConnectionStatusChangedHandle = LiveLinkProvider->RegisterConnStatusChangedHandle(
	FLiveLinkProviderConnectionStatusChanged::FDelegate::CreateStatic(&OnConnectionStatusChanged));


	//Initialize Subjects Array
	Subjects.Reserve(1);
	Subjects.Add(FName(TEXT("OscHead")));

	// Mac OSC List ///////
	ShapeNames.Reserve(55);
	ShapeNames.Add(FName(TEXT("BrowInnerUp")));
	ShapeNames.Add(FName(TEXT("BrowDownLeft")));
	ShapeNames.Add(FName(TEXT("BrowDownRight")));
	ShapeNames.Add(FName(TEXT("BrowOuterUpLeft")));
	ShapeNames.Add(FName(TEXT("BrowOuterUpRight")));
	ShapeNames.Add(FName(TEXT("EyeLookUpLeft")));
	ShapeNames.Add(FName(TEXT("EyeLookUpRight")));
	ShapeNames.Add(FName(TEXT("EyeLookDownLeft")));
	ShapeNames.Add(FName(TEXT("EyeLookDownRight")));
	ShapeNames.Add(FName(TEXT("EyeLookInLeft")));
	ShapeNames.Add(FName(TEXT("EyeLookInRight")));
	ShapeNames.Add(FName(TEXT("EyeLookOutLeft")));
	ShapeNames.Add(FName(TEXT("EyeLookOutRight")));
	ShapeNames.Add(FName(TEXT("EyeBlinkLeft")));
	ShapeNames.Add(FName(TEXT("EyeBlinkRight")));
	ShapeNames.Add(FName(TEXT("EyeSquintLeft")));
	ShapeNames.Add(FName(TEXT("EyeSquintRight")));
	ShapeNames.Add(FName(TEXT("EyeWideLeft")));
	ShapeNames.Add(FName(TEXT("EyeWideRight")));
	ShapeNames.Add(FName(TEXT("CheekPuff")));
	ShapeNames.Add(FName(TEXT("CheekSquintLeft")));
	ShapeNames.Add(FName(TEXT("CheekSquintRight")));
	ShapeNames.Add(FName(TEXT("NoseSneerLeft")));
	ShapeNames.Add(FName(TEXT("NoseSneerRight")));
	ShapeNames.Add(FName(TEXT("JawOpen")));
	ShapeNames.Add(FName(TEXT("JawForward")));
	ShapeNames.Add(FName(TEXT("JawLeft")));
	ShapeNames.Add(FName(TEXT("JawRight")));
	ShapeNames.Add(FName(TEXT("MouthFunnel")));
	ShapeNames.Add(FName(TEXT("MouthPucker")));
	ShapeNames.Add(FName(TEXT("MouthLeft")));
	ShapeNames.Add(FName(TEXT("MouthRight")));
	ShapeNames.Add(FName(TEXT("MouthRollUpper")));
	ShapeNames.Add(FName(TEXT("MouthRollLower")));
	ShapeNames.Add(FName(TEXT("MouthShrugUpper")));
	ShapeNames.Add(FName(TEXT("MouthShrugLower")));
	ShapeNames.Add(FName(TEXT("MouthClose")));
	ShapeNames.Add(FName(TEXT("MouthSmileLeft")));
	ShapeNames.Add(FName(TEXT("MouthSmileRight")));
	ShapeNames.Add(FName(TEXT("MouthFrownLeft")));
	ShapeNames.Add(FName(TEXT("MouthFrownRight")));
	ShapeNames.Add(FName(TEXT("MouthDimpleLeft")));
	ShapeNames.Add(FName(TEXT("MouthDimpleRight")));
	ShapeNames.Add(FName(TEXT("MouthUpperUpLeft")));
	ShapeNames.Add(FName(TEXT("MouthUpperUpRight")));
	ShapeNames.Add(FName(TEXT("MouthLowerDownLeft")));
	ShapeNames.Add(FName(TEXT("MouthLowerDownRight")));
	ShapeNames.Add(FName(TEXT("MouthPressLeft")));
	ShapeNames.Add(FName(TEXT("MouthPressRight")));
	ShapeNames.Add(FName(TEXT("MouthStretchLeft")));
	ShapeNames.Add(FName(TEXT("MouthStretchRight")));
	ShapeNames.Add(FName(TEXT("TongueOut")));
	ShapeNames.Add(FName(TEXT("headYaw")));
	ShapeNames.Add(FName(TEXT("headPitch")));
	ShapeNames.Add(FName(TEXT("headRoll")));
	/////////////////




	FLiveLinkStaticDataStruct StaticDataStruct(FLiveLinkBaseStaticData::StaticStruct());
	FLiveLinkBaseStaticData* BaseStaticData = StaticDataStruct.Cast<FLiveLinkBaseStaticData>();
	BaseStaticData->PropertyNames = ShapeNames;
	LiveLinkProvider->UpdateSubjectStaticData(Subjects[0], ULiveLinkBasicRole::StaticClass(), MoveTemp(StaticDataStruct));

	FTSTicker::GetCoreTicker().Tick(1.0f);

}
void FOscTrackingLiveLink::UpdateSubject()
{
	if (LiveLinkProvider.IsValid())
	{
		FLiveLinkFrameDataStruct FrameData(FLiveLinkBaseFrameData::StaticStruct());
		FLiveLinkBaseFrameData& BlendshapeData = *FrameData.Cast<FLiveLinkBaseFrameData>();
		BlendshapeData.PropertyValues = Blendshapes;
		BlendshapeData.WorldTime = FPlatformTime::Seconds();
		LiveLinkProvider->UpdateSubjectFrameData(FName(TEXT("OscHead")), MoveTemp(FrameData));
	}
}
void FOscTrackingLiveLink::ClearLiveLinkProvider() {

	if (ConnectionStatusChangedHandle.IsValid())
	{
		if (LiveLinkProvider.IsValid())
		{
			LiveLinkProvider->UnregisterConnStatusChangedHandle(ConnectionStatusChangedHandle);
		}

		ConnectionStatusChangedHandle.Reset();
	}

	if (LiveLinkProvider.IsValid())
	{
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("LiveLinkProvider References: %d\n"), LiveLinkProvider.GetSharedReferenceCount());
		FPlatformMisc::LowLevelOutputDebugString(TEXT("Deleting Live Link\n"));
	}

	FPlatformMisc::LowLevelOutputDebugString(TEXT("Live Link Provider stopped!\n"));

	if (LiveLinkProvider.IsValid()) {
		LiveLinkProvider.Reset();
	}
}

void FOscTrackingLiveLink::OSCReceivedMessageEvent(const FOSCMessage& Message, const FString& IPAddress, uint16 Port)
{
	FString StringAddress = UOSCManager::ObjectPathFromOSCAddress(Message.GetAddress());
	if (FString("/W")==StringAddress.Left(2)) {
		//Blendshapes
		int32 blendshapeIndex = 0;
		float buffer = 0.0;
		UOSCManager::GetInt32(Message.GetPacket(), 0, blendshapeIndex);
		UOSCManager::GetFloat(Message.GetPacket(), 1, buffer);
		Blendshapes[blendshapeIndex] = buffer;
		UpdateSubject();
		return;
	}
	/*else if (FString("/HRQ") == StringAddress.Left(4)) {
		//Head Rotation Quaternion
		float buffer;
		UOSCManager::GetFloat(Message.GetPacket(), 0, buffer);
		Blendshapes[53] = (buffer * -1) ;
		UOSCManager::GetFloat(Message.GetPacket(), 1, buffer);
		Blendshapes[52] = buffer;
		UOSCManager::GetFloat(Message.GetPacket(), 2, buffer);
		Blendshapes[54] = buffer;
		UpdateSubject();
		return;
	}*/
	else if (FString("/HR")==StringAddress.Left(3)) {
		//Head Rotation
		float buffer;
		float degreesToRadians = 0.0174533;
		UOSCManager::GetFloat(Message.GetPacket(), 0, buffer);
		Blendshapes[53] = (buffer * -1) * degreesToRadians;
		UOSCManager::GetFloat(Message.GetPacket(), 1, buffer);
		Blendshapes[52] = buffer * degreesToRadians;
		UOSCManager::GetFloat(Message.GetPacket(), 2, buffer);
		Blendshapes[54] = buffer * degreesToRadians;
		UpdateSubject();
		return;
	}
	//Did not find matching address pattern
}

