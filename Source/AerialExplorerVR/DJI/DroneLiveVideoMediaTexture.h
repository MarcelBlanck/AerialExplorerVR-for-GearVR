// Dennis Borgwardt, Marcel Blanck

#pragma once

#include "MediaTexture.h"
#include "DroneLiveVideoMediaTexture.generated.h"

/**
 * 
 */
UCLASS()
class AERIALEXPLORERVR_API UDroneLiveVideoMediaTexture : public UMediaTexture
#if PLATFORM_ANDROID == 1
	, public IAndroidLiveVideoSink
#endif
{
	GENERATED_BODY()

#if PLATFORM_ANDROID == 1
public:
	virtual ~UDroneLiveVideoMediaTexture();

	virtual void OnVideoFrameAvailable(const void* Buffer, uint32 BufferSize, uint32 Width, uint32 Height, uint32 DurationMs, uint32 TimeMs);

	void StartVideoStreamProcessing();

	void StopVideoStreamProcessing();
#endif

	TEnumAsByte<EPixelFormat> GetFormat() const
	{
		return PF_R8G8B8A8;
	}

	void Yuv2Rgba(const unsigned char* BufferYUV, uint32 Width, uint32 Height);

private:
	unsigned char BufferRGBA[640 * 480 * 4];
};
