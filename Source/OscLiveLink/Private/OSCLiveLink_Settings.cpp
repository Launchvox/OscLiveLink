// ©2021-2022, Launchvox, Inc. All rights reserved.


#include "OSCLiveLink_Settings.h"
#include "OscLiveLink.h"

UOSCLiveLink_Settings::UOSCLiveLink_Settings()
{
	UOSCLiveLink_Settings::ListeningAddress = FString("0.0.0.0");
	UOSCLiveLink_Settings::ListeningPort = 9001;
	UOSCLiveLink_Settings::RotationFormat = EOscRotationFormat::Quaternion;
}

#if WITH_EDITOR
void UOSCLiveLink_Settings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (
		(PropertyName == GET_MEMBER_NAME_CHECKED(UOSCLiveLink_Settings, ListeningAddress))
		||	(PropertyName == GET_MEMBER_NAME_CHECKED(UOSCLiveLink_Settings, ListeningPort))
		||	(PropertyName == GET_MEMBER_NAME_CHECKED(UOSCLiveLink_Settings, RotationFormat))
		)
	{
		//Force the server restart.
		if(IModularFeatures::Get().IsModularFeatureAvailable(FOscTrackingLiveLink::GetModularFeatureName())){
			FOscTrackingLiveLink Feature = IModularFeatures::Get().GetModularFeature<FOscTrackingLiveLink>(FOscTrackingLiveLink::GetModularFeatureName());
			Feature.OnServerSettingsChanged();		
		}
	}
}
#endif