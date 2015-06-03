// Dennis Borgwardt, Marcel Blanck

#pragma once

#include "MediaTexture.h"
#include "DroneLiveVideoMediaTexture.generated.h"

/**
 * 
 */
UCLASS()
class AERIALEXPLORERVR_API UDroneLiveVideoMediaTexture : public UMediaTexture
{
	GENERATED_BODY()


	static UDroneLiveVideoMediaTexture* Instance;
	
};
