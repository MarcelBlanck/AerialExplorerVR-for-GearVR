// Dennis Borgwardt, Marcel Blanck

#pragma once

#include "GameFramework/Actor.h"
#include "RotatingVideoScreenActor.generated.h"

UCLASS()
class AERIALEXPLORERVR_API ARotatingVideoScreenActor : public AActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Screen Control")
	void SetTargetRotation(FRotator const & NewTargetRotation, float Speed);

	UFUNCTION(BlueprintCallable, Category = "Screen Control")
	void ChangeScreenDistance(float DistanceDelta, float Speed);
	
	UPROPERTY(Category = Drone, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ScreenMesh;

	ARotatingVideoScreenActor(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	
	virtual void Tick( float DeltaSeconds ) override;

private:
	class UMaterial* MediaTextureMaterial;

    class UDroneLiveVideoMediaTexture* MediaTexture;	

	FRotator TargetRotation;
	float FollowTargetRotationSpeed;
	
	float TargetScreenDistance;
	float ScreenDistanceChangeSpeed;
};
