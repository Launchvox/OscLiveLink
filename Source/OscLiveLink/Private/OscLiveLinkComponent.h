// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//#include "RequiredProgramMainCPPInclude.h"
#include "Misc/CommandLine.h"
#include "Misc/DateTime.h"
#include "Async/TaskGraphInterfaces.h"
#include "Features/IModularFeatures.h"
#include "INetworkMessagingExtension.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Modules/ModuleManager.h"
#include "UObject/Object.h"
#include "Misc/ConfigCacheIni.h"
#include "Roles/LiveLinkBasicRole.h"
#include "Roles/LiveLinkBasicTypes.h"
#include "LiveLinkProvider.h"
#include "LiveLinkRefSkeleton.h"
#include "LiveLinkTypes.h"
#include "Misc/OutputDevice.h"

#include "Components/ActorComponent.h"
#include "OscLiveLinkComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UOscLiveLinkComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOscLiveLinkComponent();

	TSharedPtr<ILiveLinkProvider> LiveLinkProvider;

	FDelegateHandle ConnectionStatusChangedHandle;
	TSharedPtr<UOscLiveLinkComponent> OscLiveLinkComponent;
	TArray<FName> Subjects;
	TArray<FName> ShapeNames;

private:
	int BlendshapeCount = 51;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	static void OnConnectionStatusChanged();

	UFUNCTION(BlueprintCallable, Category = "LiveLink")
	void InitializeSubject();

	void ClearLiveLinkProvider();

	// Called when the component is destroyed
	virtual void BeginDestroy() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "LiveLink")
	bool UpdateSubject(TArray<float> blendshapes);
	
};
