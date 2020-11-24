# Xngine
Game Engine developed for the Master of Advanced Programming for AAA Videogames.

##### Assignment 1
For our first assignment, we aim to deliver a simple geometry viewer supporting drag and drop and orbital
camera. The final release should load the provided model (baker_house.zip) plus other models of our
choice.

The engine (now) only supports the engine camera and the render of a single model with only one diffuse texture (can have multiple meshes).

Drag and Drop a model file (.fbx) to the window to load a new model.

Some imgui windows are included that contain info about the Engine and the model loaded, aswell as some controls.


### Short description of the engine and link to the github.com page of the project
### Developed by:
Francesc Porta Solsona ([FrancPS](https://github.com/FrancPS))
### How to use:
 - Move Camera: WASD or from Configuration window.
 - Rotate Camera: ArrowKeys or RMB
 - Orbit around loaded model: LAlt + LMB
 - Focus model: F
 - Toggle Fullscreen and Borderless: from Configuration Window
 
### Additional funcionality in the engine outside of the assignment requirements:
- Automatically detects the origin of a texture coordinates, and behave accordingly.
- A Broadcast Event system is included, to detach modules between them.
- The fullscreen is toggled using SDL_WINDOW_FULLSCREEN_DESKTOP, not the "fake" fullscreen.
- The LOG definition has been edited, to print only the name of the file, and not the full path. -> Cleaner Console!
- A Start() function is included to the Modules, but not used (for now).
- Custom LookAt for the camera, with x,y,z values as parameters.
- Inputs from imgui and SDL are controlled and processed depending on whether the mouse is hovering an imgui window.
- It has been developed with lots of love and passion.

### Additional comments for the teacher to understand some part of the engine
