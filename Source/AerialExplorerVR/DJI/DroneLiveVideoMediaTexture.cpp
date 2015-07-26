// Dennis Borgwardt, Marcel Blanck

#include "AerialExplorerVR.h"

#include "DroneLiveVideoMediaTexture.h"

#if PLATFORM_ANDROID == 1
UDroneLiveVideoMediaTexture::~UDroneLiveVideoMediaTexture()
{
	FAndroidMisc::SetAndroidLiveVideoSink(nullptr);

	ClearColor = FLinearColor::Black;
	VideoTrackIndex = 0U;
	SRGB = false;
	NeverStream = true;
	
	UpdateResource();
}

void UDroneLiveVideoMediaTexture::OnVideoFrameAvailable(const void* Buffer, uint32 BufferSize, uint32 Width, uint32 Height, uint32 DurationMs, uint32 TimeMs)
{
	Yuv2Rgba(static_cast<const unsigned char *>(Buffer), Width, Height);
	UE_LOG(Generic, Warning, TEXT("UDroneLiveVideoMediaTexture::OnVideoFrameAvailable after conversion %p %d - %d %d %d %d"), Buffer, BufferSize, (int32)BufferRGBA[5], (int32)BufferRGBA[6], (int32)BufferRGBA[7], (int32)BufferRGBA[8]);
	OnCustomVideoFrameAvailable(static_cast<const void *>(BufferRGBA), (640 * 480 * 4), 640, 480, DurationMs, TimeMs);
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

void UDroneLiveVideoMediaTexture::Yuv2Rgba(const unsigned char* BufferYUV, uint32 Width, uint32 Height)
{
	// At the moment the underlying functionality is hardcoded to 640*480
	check(Width == 640 && Height == 480);

	int32 PlaneSizeY = Width * Height;
	int32 LineBytes = Width * 4;
	unsigned char Y0, Y1, Y2, Y3, U, V;
	int32 Offset, NextLineOffset;

	for (int i = 0, k = 0; i < PlaneSizeY; i += 2, k += 2)
	{
		Y0 = BufferYUV[i];
		Y1 = BufferYUV[i + 1];
		Y2 = BufferYUV[Width + i];
		Y3 = BufferYUV[Width + i + 1];

		U = BufferYUV[PlaneSizeY + k];
		V = BufferYUV[PlaneSizeY + k + 1];

		Offset = i * 4;
		
		BufferRGBA[Offset] = Y0;
		BufferRGBA[Offset + 1] = U;
		BufferRGBA[Offset + 2] = V;
		BufferRGBA[Offset + 4] = Y1;
		BufferRGBA[Offset + 5] = U;
		BufferRGBA[Offset + 6] = V;

		NextLineOffset = LineBytes + Offset;

		BufferRGBA[NextLineOffset] = Y2;
		BufferRGBA[NextLineOffset + 1] = U;
		BufferRGBA[NextLineOffset + 2] = V;
		BufferRGBA[NextLineOffset + 4] = Y3;
		BufferRGBA[NextLineOffset + 5] = U;
		BufferRGBA[NextLineOffset + 6] = V;

		if (i != 0 && (i + 2) % Width == 0)
		{
			i += Width;
		}
	}
}


