// Dennis Borgwardt, Marcel Blanck

#include "AerialExplorerVR.h"
#include "DroneControlCharacter.h"
#include "../DJI/DroneLiveVideoMediaTexture.h"

ADroneControlCharacter::ADroneControlCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	MediaTexture(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Camera"));
	Camera->AttachParent = RootComponent;
	Camera->bUsePawnControlRotation = 1U;	
	
	SpringArm = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("SpringArm"));
	SpringArm->AttachParent = RootComponent;
	SpringArm->SetWorldRotation(FRotator(0.f, 0.f, 0.f));
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> ScreenMeshFinder(TEXT("StaticMesh'/Game/Actors/Planes/Planes_Plane_3to2.Planes_Plane_3to2'"));
	checkf(ScreenMeshFinder.Object, TEXT("Did not find StaticMesh'/Game/Actors/Planes/Planes_Plane_3to2.Planes_Plane_3to2' - Maybe the asset was moved or renamed."));
	ScreenMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Screen"));
	ScreenMesh->AttachParent = SpringArm;
	ScreenMesh->SetStaticMesh(ScreenMeshFinder.Object);
	ScreenMesh->SetMobility(EComponentMobility::Movable);
	ScreenMesh->bCastDynamicShadow = false;
	ScreenMesh->bTreatAsBackgroundForOcclusion = false;
	ScreenMesh->bGenerateOverlapEvents = 0;
	ScreenMesh->bReceivesDecals = 0;
	ScreenMesh->CastShadow = 0;
	ScreenMesh->bReturnMaterialOnMove = 0;
	ScreenMesh->bAffectDynamicIndirectLighting = 0;
	ScreenMesh->bAffectDistanceFieldLighting = 0;
	ScreenMesh->bCastStaticShadow = 0;
	ScreenMesh->bCastVolumetricTranslucentShadow = 0;
	ScreenMesh->bSelfShadowOnly = 0;
	ScreenMesh->bCastFarShadow = 0;
	ScreenMesh->bCastInsetShadow = 0;
	ScreenMesh->bCastCinematicShadow = 0;
	ScreenMesh->bCastHiddenShadow = 0;
	ScreenMesh->SetSimulatePhysics(false);
	ScreenMesh->bGenerateOverlapEvents = false;
	ScreenMesh->SetEnableGravity(false);
	ScreenMesh->BodyInstance.bAutoWeld = false;
	ScreenMesh->BodyInstance.SetInstanceNotifyRBCollision(false);
	ScreenMesh->BodyInstance.SetCollisionProfileName("NoCollision");
	ScreenMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	
	ConstructorHelpers::FObjectFinder<UMaterial> MediaTextureMatFinder(TEXT("Material'/Game/Eperimental/MAT_ExperimentalMediaPlayer.MAT_ExperimentalMediaPlayer'"));
	checkf(MediaTextureMatFinder.Object, TEXT("Did not find Material'/Game/Eperimental/MAT_ExperimentalMediaPlayer.MAT_ExperimentalMediaPlayer' - Maybe the asset was moved or renamed."));
	MediaTextureMaterial = MediaTextureMatFinder.Object;

#if PLATFORM_ANDROID == 1
	MediaTexture = NewObject<UDroneLiveVideoMediaTexture>(this, TEXT("DroneLiveVideoMediaTexture"));
	MediaTexture->AddressX = TA_Clamp;
	MediaTexture->AddressY = TA_Clamp;
	MediaTexture->ClearColor = FLinearColor::Black;
	MediaTexture->VideoTrackIndex = 0U;
#endif

	DroneApi = ObjectInitializer.CreateDefaultSubobject<UDroneApiComponent>(this, TEXT("DroneApi"));
	DroneApi->OnGimbalRotationChanged.AddDynamic(this, &ADroneControlCharacter::OnGimbalRotationChanged);
}

void ADroneControlCharacter::BeginPlay()
{
	Super::BeginPlay();
#if PLATFORM_ANDROID == 1
	UMaterialInstanceDynamic* MediaTextureMaterialInstance = UMaterialInstanceDynamic::Create(MediaTextureMaterial, this);
	MediaTextureMaterialInstance->SetTextureParameterValue("MediaTexture", Cast<UTexture>(MediaTexture));
	MediaTexture->StartVideoStreamProcessing();
	ScreenMesh->SetMaterial(0, MediaTextureMaterialInstance);
#endif
}

void ADroneControlCharacter::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

#if PLATFORM_ANDROID != 1
	// Just some debug look around capabilities for the editor
	InputComponent->BindAxis(TEXT("LookUp"), this, &ADroneControlCharacter::AddControllerPitchInput);
	InputComponent->BindAxis(TEXT("LookRight"), this, &ADroneControlCharacter::AddControllerYawInput);
#endif
}

void ADroneControlCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator HMD_DeviceRotation;
	FVector HMD_DevicePosition;
	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(HMD_DeviceRotation, HMD_DevicePosition);
	DroneApi->CommandDroneToSetGimbalRotation(HMD_DeviceRotation);
}

void ADroneControlCharacter::OnGimbalRotationChanged(FRotator GimbalRotation)
{
	SpringArm->SetWorldRotation(GimbalRotation);
}