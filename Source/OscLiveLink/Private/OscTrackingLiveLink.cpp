// ©2021-2022, Launchvox, Inc. All rights reserved.


#include "OscTrackingLiveLink.h"
#include "OscLiveLink.h"
#include "Math/Quat.h"


/*Class Constructor*/
FOscTrackingLiveLink::FOscTrackingLiveLink()
{
	Blendshapes.Init(0.0, 55);
}

/*Class Constructor*/
FOscTrackingLiveLink::~FOscTrackingLiveLink()
{
	// Should only be called during shutdown
	//check(IsEngineExitRequested());
}

/*Initialize only after the server is running so that bindings succeed*/
void FOscTrackingLiveLink::Init()
{
	IModularFeatures::Get().RegisterModularFeature(GetModularFeatureName(), this);
	
	UOSCLiveLink_Settings* Plugin_Settings = GetMutableDefault<UOSCLiveLink_Settings>();
	RotationFormat = Plugin_Settings->RotationFormat;
	
	InitializeSubject();
	OscHandle = FOscLiveLinkModule::Get().OSCServer->OnOscMessageReceivedNative.AddSP(this, &FOscTrackingLiveLink::OSCReceivedMessageEvent);
	OscHandle = FOscLiveLinkModule::Get().OSCServer->OnOscBundleReceivedNative.AddSP(this, &FOscTrackingLiveLink::OSCReceivedMessageBundleEvent);

}

/*Shutdown when the module unloads, or when the server changes*/
void FOscTrackingLiveLink::Shutdown()
{
	FOscTrackingLiveLink::ClearLiveLinkProvider();
	FOscLiveLinkModule::Get().OSCServer->OnOscMessageReceivedNative.Remove(OscHandle);
	IModularFeatures::Get().UnregisterModularFeature(GetModularFeatureName(), this);
	FCoreDelegates::OnFEngineLoopInitComplete.RemoveAll(this);
}

/* Handle reinit and live settings changes*/
void FOscTrackingLiveLink::OnServerSettingsChanged()
{
	// Get and apply reset on all Objects of class. Should only ever be one.

	FOscLiveLinkModule Module = FOscLiveLinkModule::Get();

	UOSCLiveLink_Settings* Plugin_Settings = GetMutableDefault<UOSCLiveLink_Settings>();
	RotationFormat = Plugin_Settings->RotationFormat;
	Module.InitializeOSCServer(Plugin_Settings->ListeningAddress, Plugin_Settings->ListeningPort);
	
	}

/*Setup the livelink subject*/
void FOscTrackingLiveLink::InitializeSubject()
{
	if (LiveLinkProvider.IsValid())
	{
		FPlatformMisc::LowLevelOutputDebugString(TEXT("Live Link Provider already started!\n"));
		return;
	}

	LiveLinkProvider = ILiveLinkProvider::CreateLiveLinkProvider(TEXT("HallwayTile OSC"));
	//ConnectionStatusChangedHandle = LiveLinkProvider->RegisterConnStatusChangedHandle(
	//	FLiveLinkProviderConnectionStatusChanged::FDelegate::CreateStatic(&OnConnectionStatusChanged));


	//Initialize Subjects Array
	Subjects.Reserve(1);
	Subjects.Add(FName(TEXT("OscHead")));

	// Blenshape OSC List ///////
	ShapeNames.Reserve(55);
	FName Arr[] = {
		FName(TEXT("BrowInnerUp")),
		FName(TEXT("BrowDownLeft")),
		FName(TEXT("BrowDownRight")),
		FName(TEXT("BrowOuterUpLeft")),
		FName(TEXT("BrowOuterUpRight")),
		FName(TEXT("EyeLookUpLeft")),
		FName(TEXT("EyeLookUpRight")),
		FName(TEXT("EyeLookDownLeft")),
		FName(TEXT("EyeLookDownRight")),
		FName(TEXT("EyeLookInLeft")),
		FName(TEXT("EyeLookInRight")),
		FName(TEXT("EyeLookOutLeft")),
		FName(TEXT("EyeLookOutRight")),
		FName(TEXT("EyeBlinkLeft")),
		FName(TEXT("EyeBlinkRight")),
		FName(TEXT("EyeSquintLeft")),
		FName(TEXT("EyeSquintRight")),
		FName(TEXT("EyeWideLeft")),
		FName(TEXT("EyeWideRight")),
		FName(TEXT("CheekPuff")),
		FName(TEXT("CheekSquintLeft")),
		FName(TEXT("CheekSquintRight")),
		FName(TEXT("NoseSneerLeft")),
		FName(TEXT("NoseSneerRight")),
		FName(TEXT("JawOpen")),
		FName(TEXT("JawForward")),
		FName(TEXT("JawLeft")),
		FName(TEXT("JawRight")),
		FName(TEXT("MouthFunnel")),
		FName(TEXT("MouthPucker")),
		FName(TEXT("MouthLeft")),
		FName(TEXT("MouthRight")),
		FName(TEXT("MouthRollUpper")),
		FName(TEXT("MouthRollLower")),
		FName(TEXT("MouthShrugUpper")),
		FName(TEXT("MouthShrugLower")),
		FName(TEXT("MouthClose")),
		FName(TEXT("MouthSmileLeft")),
		FName(TEXT("MouthSmileRight")),
		FName(TEXT("MouthFrownLeft")),
		FName(TEXT("MouthFrownRight")),
		FName(TEXT("MouthDimpleLeft")),
		FName(TEXT("MouthDimpleRight")),
		FName(TEXT("MouthUpperUpLeft")),
		FName(TEXT("MouthUpperUpRight")),
		FName(TEXT("MouthLowerDownLeft")),
		FName(TEXT("MouthLowerDownRight")),
		FName(TEXT("MouthPressLeft")),
		FName(TEXT("MouthPressRight")),
		FName(TEXT("MouthStretchLeft")),
		FName(TEXT("MouthStretchRight")),
		FName(TEXT("TongueOut")),
		FName(TEXT("headYaw")),
		FName(TEXT("headPitch")),
		FName(TEXT("headRoll"))
	};
	ShapeNames.Append(Arr, UE_ARRAY_COUNT(Arr));


	FLiveLinkStaticDataStruct StaticDataStruct(FLiveLinkBaseStaticData::StaticStruct());
	FLiveLinkBaseStaticData* BaseStaticData = StaticDataStruct.Cast<FLiveLinkBaseStaticData>();
	BaseStaticData->PropertyNames = ShapeNames;
	LiveLinkProvider->UpdateSubjectStaticData(Subjects[0], ULiveLinkBasicRole::StaticClass(), MoveTemp(StaticDataStruct));

	FTSTicker::GetCoreTicker().Tick(1.0f);

}

/*Triggered when OSC packages are received to update curve data*/
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

/*The subject specific shutdown process*/
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

/*Try to get index as float, fallback to int if float is not found.*/
void FOscTrackingLiveLink::GetFloatWithFallbackInt(const FOSCMessage& Message, float& buffer, uint16 index)
{
	if (UOSCManager::GetFloat(Message.GetPacket(), index, buffer) != true)
	{
		int32 intBuffer = 0;
		UOSCManager::GetInt32(Message.GetPacket(), index, intBuffer);
		buffer = intBuffer;
	}
}

/*Single message incoming delegate*/
void FOscTrackingLiveLink::OSCReceivedMessageEvent(const FOSCMessage& Message, const FString& IPAddress, uint16 Port)
{
	FString StringAddress = UOSCManager::ObjectPathFromOSCAddress(Message.GetAddress());
	
	
	if (FString("/W")==StringAddress.Left(2)) {
		//Blendshapes
		int32 blendshapeIndex = 0;
		float buffer = 0.0;
		UOSCManager::GetInt32(Message.GetPacket(), 0, blendshapeIndex);
		//Try to get index 1 as float, fallback to int if float is not found.
		GetFloatWithFallbackInt(Message, buffer, 1);
		Blendshapes[blendshapeIndex] = buffer;
		UpdateSubject();
		return;
	}
	//Head Rotation Quaternion
	else if (FString("/HRQ") == StringAddress.Left(4) && RotationFormat == EOscRotationFormat::Quaternion) {
			FQuat Quat;
			float buffer;
			GetFloatWithFallbackInt(Message, buffer, 0);
			Quat.X = buffer;
			GetFloatWithFallbackInt(Message, buffer, 1);
			Quat.Y = buffer;
			GetFloatWithFallbackInt(Message, buffer, 2);
			Quat.Z = buffer;
			GetFloatWithFallbackInt(Message, buffer, 3);
			Quat.W = buffer;
			

			FVector QuatToEuler = Quat.Euler();

			Blendshapes[52] = QuatToEuler.X;
			Blendshapes[53] = QuatToEuler.Y * -1;
			Blendshapes[54] = QuatToEuler.Z;

			UpdateSubject();
			return;
	}
	// Head roatoin Euler
	else if (FString("/HR") == StringAddress.Left(3) && RotationFormat == EOscRotationFormat::Euler) {
		float buffer;
		//Scale rotation factor from degrees to radian
		float rotationsMult = 0.0174533;
		GetFloatWithFallbackInt(Message, buffer, 0);
		Blendshapes[53] = (buffer * -1) * rotationsMult;
		GetFloatWithFallbackInt(Message, buffer, 1);
		Blendshapes[52] = buffer * rotationsMult;
		GetFloatWithFallbackInt(Message, buffer, 2);
		Blendshapes[54] = buffer * rotationsMult;
		UpdateSubject();
		return;
	}
}

/*Bundle of messages incoming delegate*/
void FOscTrackingLiveLink::OSCReceivedMessageBundleEvent(const FOSCBundle& Bundle, const FString& IPAddress, uint16 Port)
{
	Bundle.GetPacket();
	TArray<FOSCMessage> Messages = UOSCManager::GetMessagesFromBundle(Bundle);
	for (int i = 0; i < Messages.Num(); i++) {
		OSCReceivedMessageEvent(Messages[i], IPAddress, Port);
	}
}

