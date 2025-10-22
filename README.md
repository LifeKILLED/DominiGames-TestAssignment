
# Native OpenGL ES Scene Renderer

This project is a lightweight engine-like framework for rendering 3D scenes on Android using OpenGL ES. It demonstrates touch-controlled scene interaction, resource management, and an extensible architecture suitable for future expansion.  

## Features

1. **Touch-controlled Scene Rotation**  
   - Rotate one or multiple cubes using multi-touch input.  
   - Smooth, real-time interaction using Android native input events.  

2. **External Asset Management**  
   - All resources (shaders, models, scenes) are stored as external files in the `assets/` folder.  
   - Easy to modify or expand without recompiling the project.  

3. **Engine-like Architecture**  
   - Organized namespaces: `Renderer`, `Input`, etc.  
   - Classes for meshes, shaders, textures, and other rendering components.  
   - Designed for modularity and clarity.  

4. **Scene and Transform System**  
   - `Scene` class supports nested hierarchies of objects, similar to modern game engines.  
   - Lazy evaluation of transformation matrices in the `Transform` class for efficiency.  

5. **Graphics Abstraction Layer**  
   - OpenGL ES is wrapped behind a renderer interface.  
   - Designed for future support of other APIs like Vulkan or Metal.  

6. **Context Lifecycle Handling**  
   - Proper handling of app minimization and restoration.  
   - Resources are safely destroyed when the OpenGL context is lost and reloaded after context restoration.

## Usage

1. Clone the repository and open it in Android Studio.  
2. Build and run on an Android device.  
3. Use one or multiple fingers to rotate the cubes in the scene.  

## Future Improvements

- Add support for Vulkan and Metal backends.  
- Expand scene management with lights, cameras, and materials.  
- Implement physics or animation systems.  
