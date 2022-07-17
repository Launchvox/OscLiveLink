// Fill out your copyright notice in the Description page of Project Settings.


#include "OscLiveLinkComponent.h"

// Sets default values for this component's properties
UOscLiveLinkComponent::UOscLiveLinkComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

// Called when the game starts
void UOscLiveLinkComponent::BeginPlay()
{
	Super::BeginPlay();
}

 void UOscLiveLinkComponent::OnConnectionStatusChanged()
{
   // Update based on connection change can go here.
}

void UOscLiveLinkComponent::InitializeSubject()
{
	if (LiveLinkProvider.IsValid())
	{
		FPlatformMisc::LowLevelOutputDebugString(TEXT("Live Link Provider already started!\n"));
		return;
	}

	LiveLinkProvider = ILiveLinkProvider::CreateLiveLinkProvider(TEXT("HallwayTile OSC"));

	ConnectionStatusChangedHandle = LiveLinkProvider->RegisterConnStatusChangedHandle(
	FLiveLinkProviderConnectionStatusChanged::FDelegate::CreateStatic(
	&OnConnectionStatusChanged));


	//Initialize Subjects Array
	Subjects.Reserve(1);
	Subjects.Add(FName(TEXT("OscHead")));

	// Mac OSC List ///////
	/*ShapeNames.Reserve(55);
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
	ShapeNames.Add(FName(TEXT("headRoll")));*/
	/////////////////

	//From JS Lib
	// ShapeNames.Reserve(55);
	ShapeNames.Add(FName(TEXT("BrowDownLeft")));
	ShapeNames.Add(FName(TEXT("BrowDownRight")));
	ShapeNames.Add(FName(TEXT("BrowInnerUp")));
	ShapeNames.Add(FName(TEXT("BrowOuterUpLeft")));
	ShapeNames.Add(FName(TEXT("BrowOuterUpRight")));
	ShapeNames.Add(FName(TEXT("CheekSquintLeft")));
	ShapeNames.Add(FName(TEXT("CheekSquintRight")));
	ShapeNames.Add(FName(TEXT("EyeBlinkLeft")));
	ShapeNames.Add(FName(TEXT("EyeBlinkRight")));
	ShapeNames.Add(FName(TEXT("EyeLookDownLeft")));
	ShapeNames.Add(FName(TEXT("EyeLookDownRight")));
	ShapeNames.Add(FName(TEXT("EyeLookInLeft")));
	ShapeNames.Add(FName(TEXT("EyeLookInRight")));
	ShapeNames.Add(FName(TEXT("EyeLookOutLeft")));
	ShapeNames.Add(FName(TEXT("EyeLookOutRight")));
	ShapeNames.Add(FName(TEXT("EyeLookUpLeft")));
	ShapeNames.Add(FName(TEXT("EyeLookUpRight")));
	ShapeNames.Add(FName(TEXT("EyeSquintLeft")));
	ShapeNames.Add(FName(TEXT("EyeSquintRight")));
	ShapeNames.Add(FName(TEXT("EyeWideLeft")));
	ShapeNames.Add(FName(TEXT("EyeWideRight")));
	ShapeNames.Add(FName(TEXT("JawLeft")));
	ShapeNames.Add(FName(TEXT("JawOpen")));
	ShapeNames.Add(FName(TEXT("JawRight")));
	ShapeNames.Add(FName(TEXT("MouthClose")));
	ShapeNames.Add(FName(TEXT("MouthDimpleLeft")));
	ShapeNames.Add(FName(TEXT("MouthDimpleRight")));
	ShapeNames.Add(FName(TEXT("MouthFrownLeft")));
	ShapeNames.Add(FName(TEXT("MouthFrownRight")));
	ShapeNames.Add(FName(TEXT("MouthFunnel")));
	ShapeNames.Add(FName(TEXT("MouthLeft")));
	ShapeNames.Add(FName(TEXT("MouthLowerDownLeft")));
	ShapeNames.Add(FName(TEXT("MouthLowerDownRight")));
	ShapeNames.Add(FName(TEXT("MouthPressLeft")));
	ShapeNames.Add(FName(TEXT("MouthPressRight")));
	ShapeNames.Add(FName(TEXT("MouthPucker")));
	ShapeNames.Add(FName(TEXT("MouthRight")));
	ShapeNames.Add(FName(TEXT("MouthRollLower")));
	ShapeNames.Add(FName(TEXT("MouthRollUpper")));
	ShapeNames.Add(FName(TEXT("MouthShrugLower")));
	ShapeNames.Add(FName(TEXT("MouthShrugUpper")));
	ShapeNames.Add(FName(TEXT("MouthSmileLeft")));
	ShapeNames.Add(FName(TEXT("MouthSmileRight")));
	ShapeNames.Add(FName(TEXT("MouthStretchLeft")));
	ShapeNames.Add(FName(TEXT("MouthStretchRight")));
	ShapeNames.Add(FName(TEXT("MouthUpperUpLeft")));
	ShapeNames.Add(FName(TEXT("MouthUpperUpRight")));
	ShapeNames.Add(FName(TEXT("NoseSneerLeft")));
	ShapeNames.Add(FName(TEXT("NoseSneerRight")));
	ShapeNames.Add(FName(TEXT("CheekPuff")));
	ShapeNames.Add(FName(TEXT("JawForward")));
	ShapeNames.Add(FName(TEXT("TongueOut")));

	ShapeNames.Add(FName(TEXT("headYaw")));
	ShapeNames.Add(FName(TEXT("headPitch")));
	ShapeNames.Add(FName(TEXT("headRoll")));
	////


	FLiveLinkStaticDataStruct StaticDataStruct(FLiveLinkBaseStaticData::StaticStruct());
	FLiveLinkBaseStaticData* BaseStaticData = StaticDataStruct.Cast<FLiveLinkBaseStaticData>();
	BaseStaticData->PropertyNames = ShapeNames;
	LiveLinkProvider->UpdateSubjectStaticData(Subjects[0], ULiveLinkBasicRole::StaticClass(), MoveTemp(StaticDataStruct));

	FTSTicker::GetCoreTicker().Tick(1.0f);

}

void UOscLiveLinkComponent::ClearLiveLinkProvider() {

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

// Called when the component is destroyed
void UOscLiveLinkComponent::BeginDestroy()
{
		ClearLiveLinkProvider();
		Super::BeginDestroy();
}

// Called every frame
void UOscLiveLinkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UOscLiveLinkComponent::UpdateSubject(TArray<float> Blendshapes)
{
	if (LiveLinkProvider.IsValid())
	{
		FLiveLinkFrameDataStruct FrameData(FLiveLinkBaseFrameData::StaticStruct());
		FLiveLinkBaseFrameData& BlendshapeData = *FrameData.Cast<FLiveLinkBaseFrameData>();
		BlendshapeData.PropertyValues = Blendshapes;
		BlendshapeData.WorldTime = FPlatformTime::Seconds();
		LiveLinkProvider->UpdateSubjectFrameData(FName(TEXT("OscHead")), MoveTemp(FrameData));
		return true;
	} else {
		return false;
	}
}