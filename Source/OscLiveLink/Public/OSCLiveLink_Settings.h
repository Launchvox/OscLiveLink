// ©2021-2022, Launchvox, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OSCLiveLink_Settings.generated.h"

/**
 * 
 */
UENUM()
enum class EOscRotationFormat : uint8
{
	Quaternion = 0	UMETA(DisplayName = "Quaternion", ToolTip = "Treat incoming rotation values as a quaternion rotator radians."),
	Euler = 1	UMETA(DisplayName = "Degrees", ToolTip = "Treat incoming rotation values as an euler rotator in degrees."),
};


UCLASS(config=Game)
class OSCLIVELINK_API UOSCLiveLink_Settings : public UObject
{
	GENERATED_BODY()
	
public:
	UOSCLiveLink_Settings();

	/*Listening on address 0.0.0.0 will accept clients from any network. Listening on address 127.0.0.1 will accept only a Local-Host client.*/
	UPROPERTY(Config, EditAnywhere, Category = "Server Settings")
	FString ListeningAddress;

	/*The port to recieve OSC Messages from. Hallway Tile broadcasts to port 9001 by default.*/
	UPROPERTY(Config, EditAnywhere, Category = "Server Settings")
	int ListeningPort;

	/*Choose whether to treat incoming rotation values as Radians or Degrees.*/
	UPROPERTY(Config, EditAnywhere, Category = "Input Settings")
	EOscRotationFormat RotationFormat;

private:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent);
};