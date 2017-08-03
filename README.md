[![ArialExplorer VR](AerialExplorerSplash.tif)](https://github.com/MarcelBlanck/AerialExplorerVR-DJI-Developer-Challenge-2015/blob/master/Art/Splash/AerialExplorerSplash.tif)

# AerialExplorerVR-DJI-Developer-Challenge-2015

This was done for the DJI Drone Developer Challenge 2015. It was done by my friend https://github.com/boke and me.

Our Project was to move the drone camera according to head movements and have a low latency regarding the video throughput. 

To avoid simulator sickness, we not simply projected the camera view to the eyes directly, as in other solutions, but to a very customized video texture in a 3D scene. This plane followed the head movement filtered and with a little latency, this way simulator sickness was reduced to 0.

The interesting parts were the Android expansion API for the Unreal Engine and moving the command flow through the Engine to Android java code and vice versa. Customized Java code was not really supported back then in the Unreal Engine 4, so it was quite hard and is a little hacky but works.

Also having an as high as possible video frame rate was our concern. The DJI API was limited to deliver the video in Java and in a non UE4 native Format, so it needed to be injected in the render thread and very quickly converted.

So, after one month we had solved the core problems and the next work would have been to polish it up and maybe have a believable 3D scene, like inside an UFO... that would have been fun, but we were working fulltime also, so did not happen.

There were plans to do much more with this, but actually we never heard again from the DJI Developer Challenge 2015, there was no winner officially announced, but we were allowed to keep the Phantom 4 Pro Drone for 1k€, so we were happy and ok with it =)

If this code helps you in any way let me know, I would be glad to hear about it.


NOTE: Actually to build this or see the Android and UE4 changes, a patchset for the Engine is needed, I will see if I can find it again, somehow it's now lost over the years =/