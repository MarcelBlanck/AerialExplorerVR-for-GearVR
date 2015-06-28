// Dennis Borgwardt, Marcel Blanck

#pragma once

#include "GameFramework/Character.h"
#include "../DJI/DroneApiComponent.h"
#include "DroneControlCharacter.generated.h"

UCLASS()
class AERIALEXPLORERVR_API ADroneControlCharacter : public ACharacter, public IGimbalRotationChangedListener
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = Drone, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(Category = Drone, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDroneApiComponent *DroneApi;

	UPROPERTY(Category = Drone, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ARotatingVideoScreenActor* RotatingVideoScreenActor;

	ADroneControlCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void Tick( float DeltaSeconds ) override;

	void DecreaseScreenDistance();

	void IncreaseScreenDistance();

	virtual void OnGimbalRotationChanged(FRotator & GimbalRotation) override;

private:
	bool HMD_FirstUpdate;
	FRotator HMD_LastDeviceRotation;
};
