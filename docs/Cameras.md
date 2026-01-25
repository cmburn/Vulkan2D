Cameras
=======
Vulkan2D has fairly simple but powerful 2D camera features that you can use in many scenarios. All drawing
in VK2D requires a camera, and as such the renderer creates and manages a default camera with the index
`VK2D_DEFAULT_CAMERA` so users never need to create a camera of their own but may do so, as the default
camera is slightly inflexible.

Creating Cameras
----------------
Cameras may be created with `vk2dCameraCreate`. You must provide a camera spec to create the camera which is information
on where the camera is in the game world and how its orientated. The spec also contains info on where the camera is
drawn on the screen in the form of the struct members `xOnScreen`, `yOnScreen`, `wOnScreen`, and `hOnScreen`.
`xOnScreen` and `yOnScreen` specify where the camera is to be drawn on the game window and `wOnScreen` and `hOnScreen`
specify the width and height of the camera in the window. This can be used to make a split-screen effect, where you have
4 cameras, each specifying 1/4 of the game window but with different x/y offsets in the window. `vk2dCameraCreate`
returns a `VK2DCameraIndex` which the you will need to store if you wish to update the camera at a later time. If
`vk2dCameraCreate` returns `VK2D_INVALID_CAMERA` then there are no more available camera spots - the total number of
cameras that can exist at once can be changed in the file `VK2D/Constants.h`.

Updating Cameras
----------------
Using the `VK2DCameraIndex` acquired from `vk2dCameraCreate` you can call `vk2dCameraUpdate` to update the spec of a
given camera. `vk2dCameraUpdate` may be called at anytime to update a camera, but changes to the camera are
not applied until the end of the frame (`vk2dRendererPresent`); if you change it multiple times mid-frame only
the most recent changes will show up in the final image. You may also use `vk2dCameraSetState` to update a 
camera's state to one of the following:

 + `VK2D_CAMERA_STATE_NORMAL` Camera is drawn to as you would expect
 + `VK2D_CAMERA_STATE_DISABLED` Camera still exists, but it will not be drawn to
 + `VK2D_CAMERA_STATE_DELETED` Camera will be deleted and the corresponding `VK2DCameraIndex` is now invalid
 + `VK2D_CAMERA_STATE_RESET` For internal use only, do not use
 
Unlike updating a camera's spec, updating the state is applied immediately, and you can enable/disable cameras mid-frame
all you like - but locking cameras may be what you want.

The Default Camera
------------------
Since all drawing in VK2D requires a camera, the default camera is always present and can't be deleted.
It is always the size of the window (or screen if fullscreen), and if you change it, it will revert back
to being the size of the screen the next time the window size is changed or the swapchain is otherwise
forced to be recreated. Since it's always the size of the screen, it's helpful for debug info or even just
rendering your UI, assuming you handle the sizing of your own UI manually.

Locking Cameras
---------------
Normally all cameras are drawn to every time you draw something, which is useful for split screen. Should you want to
draw certain elements of the game to only particular cameras you can either disable the cameras you don't want to draw
to or just lock the renderer to a single camera. Using `vk2dRendererLockCameras` you can specify a single camera that
will be drawn to and all others will be ignored. Call `vk2dRendererUnlockCameras` to revert the renderer back to drawing
like normal.