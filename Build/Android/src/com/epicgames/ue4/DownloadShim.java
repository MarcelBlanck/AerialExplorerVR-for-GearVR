package com.epicgames.ue4;

import vr.mighty.pirates.AerialExplorerVR.OBBDownloaderService;
import vr.mighty.pirates.AerialExplorerVR.DownloaderActivity;


public class DownloadShim
{
	public static OBBDownloaderService DownloaderService;
	public static DownloaderActivity DownloadActivity;
	public static Class<DownloaderActivity> GetDownloaderType() { return DownloaderActivity.class; }
}

