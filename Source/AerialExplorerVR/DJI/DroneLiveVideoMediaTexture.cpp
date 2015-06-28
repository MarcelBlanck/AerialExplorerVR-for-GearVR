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
	Yuv2Rgba(static_cast<const unsigned char *>(Buffer), Width, Height);
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
	//check(Width == 640 && Height = 480);

	int32 RBGAOffset = 0;

	int32 frameSize = Width * Height;
	int32 y, u, v, r, g, b;
	int32 a0, a1, a2, a3, a4;

	for (int32 row = 0; row < Height; ++row)
	{
		for (int32 column = 0; column < Width; ++column)
		{
			y = BufferYUV[row * Width + column];
			v = BufferYUV[frameSize + (row >> 1) * Width + (column & ~1) + 0];
			u = BufferYUV[frameSize + (row >> 1) * Width + (column & ~1) + 1];
			
		/*	y = y < 16 ? 16 : y;

			a0 = 1192 * (y - 16);
			a1 = 1634 * (v - 128);
			a2 = 832 * (v - 128);
			a3 = 400 * (u - 128);
			a4 = 2066 * (u - 128);

			r = (a0 + a1) >> 10;
			g = (a0 - a2 - a3) >> 10;
			b = (a0 + a4) >> 10;
			*/


			r = (int32)(1.164f * (y - 16) + 1.596f * (v - 128));
			g = (int32)(1.164f * (y - 16) - 0.391f * (u - 128)) - 0.813f * (v - 128);
			b = (int32)(1.164f * (y - 16) + 2.018f * (u - 128));

			BufferRGBA[RBGAOffset] = FMath::Clamp<unsigned char>(r, 0U, 255U);
			BufferRGBA[RBGAOffset + 1] = FMath::Clamp<unsigned char>(g, 0U, 255U);
			BufferRGBA[RBGAOffset + 2] = FMath::Clamp<unsigned char>(b, 0U, 255U);
			BufferRGBA[RBGAOffset + 3] = 255U;

			RBGAOffset += 4;
		}
	}
}


