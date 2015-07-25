// Dennis Borgwardt, Marcel Blanck

#include "AerialExplorerVR.h"
#include "RotatingVideoScreenActor.h"
#include "../DJI/DroneLiveVideoMediaTexture.h"

ARotatingVideoScreenActor::ARotatingVideoScreenActor(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	MediaTextureMaterial(nullptr),
	MediaTexture(nullptr),
	TargetRotation(0.f,0.f,0.f),
	FollowTargetRotationSpeed(1.f),
	TargetScreenDistance(300.f),
	ScreenDistanceChangeSpeed(1.f)
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Scene"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> ScreenMeshFinder(TEXT("StaticMesh'/Game/Actors/Planes/Planes_Plane_3to2.Planes_Plane_3to2'"));
	checkf(ScreenMeshFinder.Object, TEXT("Did not find StaticMesh'/Game/Actors/Planes/Planes_Plane_3to2.Planes_Plane_3to2' - Maybe the asset was moved or renamed."));
	ScreenMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Screen"));
	ScreenMesh->AttachParent = RootComponent;
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
	ScreenMesh->SetRelativeRotation(FRotator(0.f, -90.f, -90.f));
	ScreenMesh->SetRelativeLocation(FVector(TargetScreenDistance, 0.f, 0.f));

	ConstructorHelpers::FObjectFinder<UMaterial> MediaTextureMatFinder(TEXT("Material'/Game/Eperimental/MAT_NV21_TO_RGB_OneTex.MAT_NV21_TO_RGB_OneTex'"));
	checkf(MediaTextureMatFinder.Object, TEXT("Did not find Material'/Game/Eperimental/MAT_NV21_TO_RGB_OneTex.MAT_NV21_TO_RGB_OneTex' - Maybe the asset was moved or renamed."));
	MediaTextureMaterial = MediaTextureMatFinder.Object;

#if PLATFORM_ANDROID == 1
	MediaTexture = NewObject<UDroneLiveVideoMediaTexture>(this, TEXT("DroneLiveVideoMediaTexture"));
	MediaTexture->AddressX = TA_Clamp;
	MediaTexture->AddressY = TA_Clamp;
	MediaTexture->ClearColor = FLinearColor::Black;
	MediaTexture->VideoTrackIndex = 0U;
#endif
}

void ARotatingVideoScreenActor::BeginPlay()
{
	Super::BeginPlay();

	ScreenMesh->SetRelativeRotation(FRotator(0.f, -90.f, -90.f));
	ScreenMesh->SetRelativeLocation(FVector(TargetScreenDistance, 0.f, 0.f));

#if PLATFORM_ANDROID == 1
	UMaterialInstanceDynamic* MediaTextureMaterialInstance = UMaterialInstanceDynamic::Create(MediaTextureMaterial, this);
	MediaTextureMaterialInstance->SetTextureParameterValue("MediaTexture", Cast<UTexture>(MediaTexture));
	MediaTexture->StartVideoStreamProcessing();
	ScreenMesh->SetMaterial(0, MediaTextureMaterialInstance);
#endif
}

void ARotatingVideoScreenActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, FollowTargetRotationSpeed);
	SetActorRotation(NewRotation);

	FVector ScreenMeshLocation = ScreenMesh->RelativeLocation;
	ScreenMeshLocation.X = FMath::Lerp(ScreenMesh->RelativeLocation.X, TargetScreenDistance, FMath::Clamp(DeltaTime * ScreenDistanceChangeSpeed, 0.f, 1.f));
	ScreenMesh->SetRelativeLocation(ScreenMeshLocation);
}

void ARotatingVideoScreenActor::SetTargetRotation(FRotator const & NewTargetRotation, float Speed)
{
	TargetRotation = NewTargetRotation;
	FollowTargetRotationSpeed = FMath::Clamp(Speed, 0.f, 1000.f);
}

void ARotatingVideoScreenActor::ChangeScreenDistance(float DistanceDelta, float Speed)
{
	TargetScreenDistance = FMath::Clamp(TargetScreenDistance + DistanceDelta, 0.f, 2000.f);
	ScreenDistanceChangeSpeed = FMath::Clamp(Speed, 0.f, 1000.f);
}
