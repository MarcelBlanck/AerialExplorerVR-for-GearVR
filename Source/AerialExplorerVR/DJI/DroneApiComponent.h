// Dennis Borgwardt, Marcel Blanck

#pragma once

#include "Components/ActorComponent.h"
#include "Math/Rotator.h"
#include "DroneApiComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AERIALEXPLORERVR_API UDroneApiComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGimbalRotationChangedDelegate, FRotator, GimbalRotation);

public:	
	UFUNCTION(BlueprintCallable, Category = "Drone Control")
	void CommandDroneToSetGimbalRotation(FRotator const & GimbalRotation);

	UPROPERTY(Category = Debug, VisibleAnywhere, BlueprintReadWrite)
	FRotator DebugFakeGimbalRotation;

	UDroneApiComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;	

	/* Delegate to execute when the drone reports a changed GimbalRotation */
	UPROPERTY(BlueprintAssignable)
	FGimbalRotationChangedDelegate OnGimbalRotationChanged;

private:
	FRotator LastGimbalRotation;
	void UpdateGimbalRotation();
};
