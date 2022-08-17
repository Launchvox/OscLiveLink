// ©2021-2022, Launchvox, Inc. All rights reserved.


#include "OSCLiveLink_Settings.h"

UOSCLiveLink_Settings::UOSCLiveLink_Settings()
{
	UOSCLiveLink_Settings::ListeningAddress = FString("0.0.0.0");
	UOSCLiveLink_Settings::ListeningPort = 9001;
	UOSCLiveLink_Settings::RotationFormat = EOscRotationFormat::Radians;
}
