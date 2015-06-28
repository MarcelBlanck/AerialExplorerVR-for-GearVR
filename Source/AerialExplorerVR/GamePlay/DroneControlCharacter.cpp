// Dennis Borgwardt, Marcel Blanck

#include "AerialExplorerVR.h"
#include "DroneControlCharacter.h"
#include "RotatingVideoScreenActor.h"

ADroneControlCharacter::ADroneControlCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	Camera(nullptr),
	DroneApi(nullptr),
	RotatingVideoScreenActor(nullptr),
	HMD_FirstUpdate(true),
    HMD_LastDeviceRotation()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Camera"));
	Camera->AttachParent = RootComponent;
	Camera->bUsePawnControlRotation = 1U;	

	DroneApi = ObjectInitializer.CreateDefaultSubobject<UDroneApiComponent>(this, TEXT("DroneApi"));
	DroneApi->SetGimbalRotationChangedListener(this);
}

void ADroneControlCharacter::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ARotatingVideoScreenActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		RotatingVideoScreenActor = *ActorItr;
		break;
	}

	// DroneApi->OnGimbalRotationChanged.AddDynamic(this, &ADroneControlCharacter::OnGimbalRotationChanged);
}

void ADroneControlCharacter::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

#if PLATFORM_ANDROID != 1
	// Just some debug look around capabilities for the editor
	InputComponent->BindAxis(TEXT("LookUp"), this, &ADroneControlCharacter::AddControllerPitchInput);
	InputComponent->BindAxis(TEXT("LookRight"), this, &ADroneControlCharacter::AddControllerYawInput);
	InputComponent->BindAction(TEXT("ScreenNearer"), EInputEvent::IE_Pressed,this, &ADroneControlCharacter::DecreaseScreenDistance);
	InputComponent->BindAction(TEXT("ScreenFarer"), EInputEvent::IE_Pressed, this, &ADroneControlCharacter::IncreaseScreenDistance);
#endif
}

void ADroneControlCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator HMD_DeviceRotation;
	FVector HMD_DevicePosition;
	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(HMD_DeviceRotation, HMD_DevicePosition);
	if (HMD_DeviceRotation != HMD_LastDeviceRotation || HMD_FirstUpdate)
	{
		DroneApi->CommandDroneToSetGimbalRotation(HMD_DeviceRotation);
		HMD_LastDeviceRotation = HMD_DeviceRotation;
		HMD_FirstUpdate = false;
	}

#if PLATFORM_ANDROID != 1
	if (RotatingVideoScreenActor != nullptr)
	{
		RotatingVideoScreenActor->SetTargetRotation(DroneApi->DebugFakeGimbalRotation, 1.f);
	}
#endif	
}

void ADroneControlCharacter::OnGimbalRotationChanged(FRotator & GimbalRotation)
{
#if PLATFORM_ANDROID == 1
	UE_LOG(Generic, Warning, TEXT("Gimbal Rotation Delegated: %f Roll: %f Yaw: %f"),
		GimbalRotation.Pitch, GimbalRotation.Roll, GimbalRotation.Yaw);
	if (RotatingVideoScreenActor != nullptr)
	{
		// TODO Check that speed is slightly higher than gimbal rotation velocity
		// Shouldn't overshoot do to contant updating.
		UE_LOG(Generic, Warning, TEXT("Gimbal SetTargetRotation of screen: %f Roll: %f Yaw: %f"),
			GimbalRotation.Pitch, GimbalRotation.Roll, GimbalRotation.Yaw);
		RotatingVideoScreenActor->SetTargetRotation(GimbalRotation, 1.f);
	}
#endif
}

void ADroneControlCharacter::DecreaseScreenDistance()
{
	RotatingVideoScreenActor->ChangeScreenDistance(-20.f, 1.f);
	UE_LOG(Generic, Warning, TEXT("DecreaseScreenDistance()"));
}

void ADroneControlCharacter::IncreaseScreenDistance()
{
	RotatingVideoScreenActor->ChangeScreenDistance(20.f, 1.f);
	UE_LOG(Generic, Warning, TEXT("IncreaseScreenDistance()"));
}