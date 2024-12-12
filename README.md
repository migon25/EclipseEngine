# EclipseEngine
A 3D game engine made from scratch by 4 university students for the Game Engines subject.

## The Team
This team is composed by:
- **Marc Gallardo**: [https://github.com/Marchusky](https://github.com/Marchusky)
- **Marc San José**: [https://github.com/marcsjm19](https://github.com/marcsjm19)
- **Miguel Tamaño**: [https://github.com/migon25](https://github.com/migon25)
- **Oscar Escofet**: [https://github.com/OscarEFO](https://github.com/OscarEFO)

## New Features 
- **Street Enviroment Scene is automatically loaded when starting the engine**
- **Transformation and Game Object Hierarchy**
  - The user can use the inspector to modify a game object; Delete, reparent, drop any mesh or texture...
- **Improved Resource Management**
  - Assets window that shows all assets.
  - The user can drop/import new files and the assets window reacts properly.
  - Asset files can be deleted through the window.
- **File System**
  - Scene can be serialized to a file that can be opened and saved.
  - The user can recover the simulation to a previous save.

## Key Functionalities
- **File Handling**:
  - Supports drag-and-drop of **FBX** files from any location on the hard drive.
  - Accepts **DDS/PNG** files for texture application to selected GameObjects.

- **GameObject Structure**:
  - Each loaded mesh creates a **GameObject** with three display-only components: **Transform**, **Mesh**, and **Texture**.

- **Camera Controls**:
  - **Right-click + WASDQE**: FPS-like movement and free look.
  - **Mouse Wheel**: Zoom in and out.
  - **Alt + Left Click**: Orbit the object.
  - Press **“F”**: Focus the camera around the geometry.
  - Holding **SHIFT**: Increases movement speed.

- **Editor Windows**:
  - **Console**:
    - Logs the geometry loading process from ASSIMP and external library initialization.
  - **Configuration**:
    - Graph for FPS display.
    - Adjustable settings for renderer, window, input, and textures.
    - Displays memory consumption, hardware detection, and software version information (SDL, OpenGL, DevIL).
  - **Hierarchy**:
    - Lists all GameObjects; allows selection of GameObjects or changes to parent relations of GameObjects.
  - **Inspector**:
    - Displays component details of the selected GameObject:
      - **Transform**: Position, rotation, and scale (read-only).
      - **Mesh**: Mesh details with an option to show normals (per-triangle and per-face).
      - **Texture**: Shows texture size and path, with an option for a checkered texture view.

- **General Menu**:
  - Options to quit the engine, visit the GitHub page, and view engine information (“About”).
  - Ability to turn features on/off.

## How to Use the Engine
1. Drag and drop **FBX** files and **DDS/PNG** textures into the engine.
2. Use the **camera controls** for navigating the scene.
3. Access **Editor Windows** for GameObject management and detailed information.
4. Edit and transform the **FBX** files using the move, rotate or scale options.
5. Save the file to recover your edits or return to a previous state of the scene.