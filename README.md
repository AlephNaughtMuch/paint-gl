# paint-gl

A **real-time renderer built in C++ with OpenGL**, exploring the modern GPU pipeline through the lens of stylized, painterly rendering. The project covers deferred rendering, GLSL shader programming, and screen-space post-processing while building toward a renderer with a distinctive visual identity informed by years of working in VFX and film production.

---

## 🎬 Demo

![paint-gl Layer 1 Demo](assets/demo_phong_helmet.gif)

*Damaged Helmet rendered with Phong lighting and texture -- Layer 1 milestone.*

---

## 💼 Why This Project Exists

Graphics programming sits at the intersection of technical depth and visual craft. Coming from a background in VFX, film lighting, and compositing, this project is about deepening that intersection by building a real-time renderer from the ground up rather than working through an existing engine or API wrapper.

The progression so far:

```
CPU software rasterizer (C) -> OpenGL real-time renderer (C++) -> Vulkan
```

The software rasterizer ([softwareRenderer](https://github.com/AlephNaughtMuch/softwareRenderer)) established the pipeline from first principles - projection math, triangle rasterization, Phong shading, depth buffering - all on the CPU without any graphics API. This project moves that understanding onto the GPU, working directly against the OpenGL API to explore the same concepts in hardware.

The goal is fluency in real-time rendering with a creative point of view: understanding not just how to call the API, but why the pipeline is structured the way it is, and using that understanding to produce something that looks genuinely distinctive.

---

## 🎨 Vision

The renderer targets three visual modes:

- **Stylized film look** - toon shading, hard rim lighting, ink outlines via edge detection on the normal and depth buffers, limited colour palette
- **Painterly / expressive** - screen-space techniques that break the clean CG look, canvas texture overlays, expressive light response
- **Technical showcase** - a deferred pipeline with exposed G-buffer debug modes, normal and depth visualisation, render pass inspection

The NPR direction is a vehicle for understanding concepts that are universally applicable in real-time rendering: framebuffers, render passes, screen-space algorithms, shader architecture. The creative sensibility comes from a professional background in film lighting and compositing, applied here to real-time graphics.

---

## ✨ Current Features

### Core Setup
- GLFW Wayland window and OpenGL 4.6 core context
- GLAD function pointer loading
- Framebuffer resize handling
- Keyboard and mouse input with clean window lifecycle (ESC to exit)
- Hardware backface culling via `GL_CULL_FACE`

### Mesh Pipeline
- Custom OBJ loader with support for n-gons, missing UVs/normals, and arbitrary face formats
- Vertex deduplication via hash map cache -- 6x reduction in vertex count on dense meshes (432k to 72k on the Stanford bunny)
- Interleaved vertex buffer layout (position, normal, UV) uploaded via VAO, VBO, and EBO
- RAII-based Mesh, Shader, and Texture classes with automatic GPU resource cleanup via destructors
- Vertex and fragment shader pipeline loaded from GLSL files at runtime
- MVP matrix system (model, view, projection) passed as uniforms
- Depth testing for correct fragment ordering

### Camera System
- Free look camera with WASD movement and mouse look
- Pitch and yaw driven forward vector calculation via trigonometry
- Right vector derived from cross product of forward and up
- Pitch clamping to prevent gimbal flip
- Delta time based movement for frame rate independent speed
- Configurable FOV, near and far planes, and movement speed
- GLFW cursor capture and mouse callback via window user pointer

### Lighting
- Phong lighting model implemented in GLSL -- ambient, diffuse, and specular components
- Normal matrix transformation for correct lighting under non-uniform scaling
- Fragment position and normal interpolated across triangles via vertex shader outputs
- Configurable light position, light colour, ambient strength, diffuse strength, specular strength, and shininess
- Camera position passed as uniform for view-dependent specular calculation

### Texturing
- Texture loading via stb_image with support for PNG and JPEG
- Automatic mipmap generation for correct rendering at distance
- Linear filtering with trilinear mipmapping
- RGBA and RGB format detection based on channel count
- 1x1 white pixel fallback for untextured meshes -- Phong lighting preserved without a texture file
- Texture coordinates interpolated per fragment and combined with Phong lighting

---

## 🗺️ Planned Features

### Layer 2 - Stylized Film Look
- Deferred rendering pipeline with G-buffer
- Toon shading with configurable light bands
- Hard rim lighting
- Edge detection via normal and depth buffer derivatives
- Limited colour palette via fragment shader quantisation
- G-buffer debug visualisation modes

### Layer 3 - Painterly / Expressive
- Screen-space canvas texture overlay
- Brush stroke simulation
- Expressive NPR post-processing passes
- Temporal techniques for hand-drawn feel

---

## 📚 Concepts Covered

### OpenGL Pipeline
- OpenGL 4.6 core profile context creation and GLAD function pointer loading
- Vertex Array Objects (VAO), Vertex Buffer Objects (VBO), Element Buffer Objects (EBO)
- Interleaved vertex buffer layout and `glVertexAttribPointer` attribute description
- Indexed drawing with `glDrawElements`
- Depth testing with `GL_DEPTH_TEST`
- Hardware backface culling with `GL_CULL_FACE`

### Shaders
- GLSL vertex and fragment shaders loaded and compiled at runtime
- Shader program linking and error reporting
- Uniform variables for MVP matrices, lighting properties, and per-frame camera data
- Vertex shader outputs interpolated across fragments via `in`/`out` variables

### Lighting
- Phong lighting model -- ambient, diffuse, and specular components
- Normal matrix for correct normal transformation under non-uniform scaling
- View-dependent specular via camera position uniform
- Per-fragment lighting in GLSL fragment shader

### Texturing
- Texture objects, binding, and texture units in OpenGL
- stb_image for PNG/JPEG loading with vertical flip correction
- Mipmap generation and trilinear filtering
- `sampler2D` uniform and `texture()` GLSL function
- Combining texture colour with Phong lighting in the fragment shader
- Defensive fallback for missing texture files

### Camera and Math
- Free look camera with pitch, yaw, and derived vectors
- Forward vector calculation from Euler angles via trigonometry
- Cross product for right vector derivation
- Model, view, and projection matrix pipeline
- Perspective projection and look-at view matrix via GLM
- Delta time for frame rate independent movement

### Input
- GLFW keyboard polling for WASD movement
- GLFW mouse callback for look input
- Window user pointer for passing application state to callbacks
- Mouse offset calculation with first-frame guard

### C++ Patterns
- RAII for GPU resource management via constructors and destructors
- Class design for Shader, Mesh, Camera, and Texture
- Enum class for type-safe movement directions
- File parsing with `std::ifstream`, `std::istringstream`, and `std::stringstream`
- Static local variables for persistent callback state
- `std::unordered_map` for O(1) vertex deduplication during mesh loading

*(Updated as the project progresses)*

---

## 🔧 Libraries

| Library | Purpose | Why |
|---|---|---|
| [GLFW](https://www.glfw.org/) | Window and OpenGL context | Minimal, focused purely on windowing with no unnecessary abstraction |
| [GLAD](https://glad.dav1d.de/) | OpenGL function pointer loading | Single-file code generator, drops directly into the project |
| [GLM](https://github.com/g-truc/glm) | Vector and matrix math | Header-only, mirrors GLSL types exactly |
| [stb_image](https://github.com/nothings/stb) | Texture loading | Single header, handles PNG/JPEG without a heavy dependency |

No engine, no scene graph, no OpenGL abstraction layer. Everything is written directly against the OpenGL API.

---

## 📖 Learning Sources

- [LearnOpenGL](https://learnopengl.com/) - primary OpenGL reference
- GLFW and OpenGL documentation

---

## 📁 Project Structure

```
paint-gl/
├── src/                  # C++ source files
│   ├── main.cpp
│   ├── camera.cpp
│   ├── framebuffer.cpp
│   ├── input.cpp
│   ├── obj_loader.cpp
│   ├── shader.cpp
│   └── texture.cpp
├── include/              # Header files
│   ├── camera.h
│   ├── framebuffer.h
│   ├── input.h
│   ├── mesh.h
│   ├── obj_loader.h
│   ├── shader.h
│   ├── texture.h
│   └── vertex.h
├── shaders/              # GLSL vertex and fragment shaders
│   ├── default.vert
│   └── default.frag
├── assets/               # OBJ meshes and textures
├── third_party/
│   ├── glad/             # GLAD loader (OpenGL 4.6 core)
│   └── stb/              # stb_image
└── Makefile
```

---

## 🛠️ Building and Running

### Requirements
- Linux (developed on Arch Linux with Wayland / Hyprland)
- GCC / G++ with C++17 support
- OpenGL 4.6 compatible GPU and drivers
- GLFW Wayland (`glfw-wayland`) and GLM installed via pacman

### Build

```bash
make
```

### Run

```bash
make run
```

### Clean

```bash
make clean
```

---

## 🎨 Asset Credits

- **Damaged Helmet** by [cjsliuj](https://sketchfab.com/cjsliuj) on Sketchfab, licensed under [CC Attribution](https://creativecommons.org/licenses/by/4.0/)
