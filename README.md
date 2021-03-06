# Xngine
Game Engine developed for the Master of Advanced Programming for AAA Videogames.
([Github Project Link](https://github.com/FrancPS/Xngine))

##### Assignment 1
For our first assignment, we aim to deliver a simple geometry viewer supporting drag and drop and orbital
camera. The final release should load the provided model (baker_house.zip) plus other models of our
choice.

The engine (now) only supports the engine camera and the render of a single model with only one diffuse texture (can have multiple meshes).

Drag and Drop a model file to the window to load a new model. Dropping an image file changes the texture of the loaded model.

Some imgui windows are included that contain info about the Engine and the model loaded, aswell as some controls.
### Developed by:
Francesc Porta Solsona ([FrancPS](https://github.com/FrancPS))
### How to use:
 - Move Camera: WASD or from Configuration window.
 - Rotate Camera: ArrowKeys or RMB
 - Orbit around loaded model: LAlt + LMB
 - Focus model: F
 - Toggle Fullscreen and Borderless: from Configuration Window
 - Close Application: ESC, Window X button, or from File>Quit App
### Additional funcionality in the engine outside of the assignment requirements:
- Automatically detects the origin of a texture coordinates, and behave accordingly.
- A Broadcast Event system is included, to detach modules between them.
- The LOG definition has been edited, to print only the name of the file, and not the full path. -> Cleaner Console!
- Custom LookAt for the camera, with x,y,z values as parameters.
- Inputs from imgui and SDL are controlled and processed depending on whether the mouse is hovering an imgui window.
- It has been developed with lots of love and passion.
### Additional comments for the teacher to understand some part of the engine
I have encountered an error that generates when you Drag&Drop a new model the second time (so when doing two drag&drops, on the second).
(Note it will only appear in Debug mode)
The error is encounterd by `glDebugMessageCallback` and prints as this:
`ModuleRender.cpp(42) : <Source:API> <Type:Error> <Severity:high> <ID:1281> <Message:Error has been generated. GL error GL_INVALID_VALUE in (null): (ID: 173538523) Generic error>`

By debugging, I have found that it originates in *ModuleDebugDraw.cpp*, explicitly in line:
```C++
87: glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(dd::DrawVertex), lines);
```
Since we are probably not going to use debug-draw in the future, and the program still runs properly, i decided to ignore this message.
