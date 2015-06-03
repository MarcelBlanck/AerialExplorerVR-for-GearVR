// Dennis Borgwardt, Marcel Blanck

#include "AerialExplorerVR.h"

#include "DroneLiveVideoMediaTexture.h"

#if PLATFORM_ANDROID == 1
UDroneLiveVideoMediaTexture::~UDroneLiveVideoMediaTexture()
{
	FAndroidMisc::SetAndroidLiveVideoSink(nullptr);
}

void UDroneLiveVideoMediaTexture::OnVideoFrameAvailable(const void* Buffer, uint32 BufferSize, uint32 Width, uint32 Height, uint32 DurationMs, uint32 TimeMs)
{
	UE_LOG(Generic, Warning, TEXT("UDroneLiveVideoMediaTexture::OnVideoFrameAvailable %p %d"), Buffer, BufferSize);
	OnCustomVideoFrameAvailable(Buffer, BufferSize, Width, Height, DurationMs, TimeMs);
}

void UDroneLiveVideoMediaTexture::StartVideoStreamProcessing()
{
	FAndroidMisc::SetAndroidLiveVideoSink(this);
}

void UDroneLiveVideoMediaTexture::StopVideoStreamProcessing()
{
	FAndroidMisc::SetAndroidLiveVideoSink(nullptr);
}
#endif


