// Dennis Borgwardt, Marcel Blanck

#include "AerialExplorerVR.h"
#include "DroneApiComponent.h"

FAndroidMisc::FGimbalRotation * DebugFakeGimbalRotation = new FAndroidMisc::FGimbalRotation;

UDroneApiComponent::UDroneApiComponent() :
	LastGimbalRotation(ForceInitToZero)
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UDroneApiComponent::BeginPlay()
{
	Super::BeginPlay();
	UpdateGimbalRotation();
}

void UDroneApiComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	UpdateGimbalRotation();
}

void UDroneApiComponent::UpdateGimbalRotation()
{
#if PLATFORM_ANDROID == 1
	FAndroidMisc::FGimbalRotation GimbalRotation = FAndroidMisc::GetDroneGimbalRotation();
	if (LastGimbalRotation.Pitch != GimbalRotation.Pitch
			|| LastGimbalRotation.Roll != GimbalRotation.Roll
			|| LastGimbalRotation.Yaw != GimbalRotation.Yaw)
	{
		LastGimbalRotation.Pitch = GimbalRotation.Pitch;
		LastGimbalRotation.Roll = GimbalRotation.Roll;
		LastGimbalRotation.Yaw = GimbalRotation.Yaw;
		OnGimbalRotationChanged.Broadcast(LastGimbalRotation);
		UE_LOG(Generic, Warning, TEXT("Gimbal Rotation Initialized to Pitch: %f Roll: %f Yaw: %f"),
			GimbalRotation.Pitch, GimbalRotation.Roll, GimbalRotation.Yaw);
	}
#else
	FRotator FakeRotator;
	FakeRotator.Pitch = (*DebugFakeGimbalRotation).Pitch;
	FakeRotator.Roll = (*DebugFakeGimbalRotation).Roll;
	FakeRotator.Yaw = (*DebugFakeGimbalRotation).Yaw;
	OnGimbalRotationChanged.Broadcast(FakeRotator);
#endif
}

void UDroneApiComponent::CommandDroneToSetGimbalRotation(FRotator const & Rotation)
{
#if PLATFORM_ANDROID == 1
	struct FAndroidMisc::FGimbalRotation GimbalRotation; 
	GimbalRotation.Pitch = Rotation.Pitch;
	GimbalRotation.Roll = Rotation.Roll;
	GimbalRotation.Yaw = Rotation.Yaw;
	FAndroidMisc::CommandDroneToSetGimbalRotation(GimbalRotation);
#else
	(*DebugFakeGimbalRotation).Pitch = Rotation.Pitch;
	(*DebugFakeGimbalRotation).Roll = Rotation.Roll;
	(*DebugFakeGimbalRotation).Yaw = Rotation.Yaw;
#endif
}