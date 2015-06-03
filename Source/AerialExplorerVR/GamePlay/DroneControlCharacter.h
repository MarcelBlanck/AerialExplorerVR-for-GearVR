// Dennis Borgwardt, Marcel Blanck

#pragma once

#include "GameFramework/Character.h"
#include "../DJI/DroneApiComponent.h"
#include "DroneControlCharacter.generated.h"

UCLASS()
class AERIALEXPLORERVR_API ADroneControlCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = Drone, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(Category = Drone, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(Category = Drone, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ScreenMesh;

	UPROPERTY(Category = Drone, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDroneApiComponent *DroneApi;

	ADroneControlCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void Tick( float DeltaSeconds ) override;

	void OnGimbalRotationChanged(FRotator GimbalRotation);

private:
	class UMaterial* MediaTextureMaterial;
	class UDroneLiveVideoMediaTexture* MediaTexture;
};
