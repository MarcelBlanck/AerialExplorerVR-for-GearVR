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
	UPROPERTY()
	UCameraComponent* Camera;

	UPROPERTY()
	USpringArmComponent* SpringArm;

	UPROPERTY()
	UStaticMeshComponent* ScreenMesh;

	UPROPERTY()
	UDroneApiComponent *DroneApi;

	ADroneControlCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void Tick( float DeltaSeconds ) override;

	void OnGimbalRotationChanged(FRotator GimbalRotation);

private:
	class UMaterial* MediaTextureMaterial;
	class UMediaTexture* MediaTexture;
};
