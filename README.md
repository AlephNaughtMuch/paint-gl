# paint-gl

A **real-time renderer built in C++ with OpenGL**, exploring the modern GPU pipeline through the lens of stylized, painterly rendering. The project covers deferred rendering, GLSL shader programming, and screen-space post-processing while building toward a renderer with a distinctive visual identity informed by years of working in VFX and film production.

---

## 💼 Why This Project Exists

Graphics programming sits at the intersection of technical depth and visual craft. Coming from a background in VFX, film lighting, and compositing, this project is about deepening that intersection by building a real-time renderer from the ground up rather than working through an existing engine or API wrapper.

The progression so far:

```
CPU software rasterizer (C) -> OpenGL real-time renderer (C++) -> Vulkan
```

The software rasterizer ([softwareRenderer](https://github.com/AlephNaughtMuch/softwareRenderer)) established the pipeline from first principles — projection math, triangle rasterization, Phong shading, depth buffering — all on the CPU without any graphics API. This project moves that understanding onto the GPU, working directly against the OpenGL API to explore the same concepts in hardware.

The goal is fluency in real-time rendering with a creative point of view: understanding not just how to call the API, but why the pipeline is structured the way it is, and using that understanding to produce something that looks genuinely distinctive.

---

## 🎨 Vision

The renderer targets three visual modes:

- **Stylized film look** — toon shading, hard rim lighting, ink outlines via edge detection on the normal and depth buffers, limited colour palette
- **Painterly / expressive** — screen-space techniques that break the clean CG look, canvas texture overlays, expressive light response
- **Technical showcase** — a deferred pipeline with exposed G-buffer debug modes, normal and depth visualisation, render pass inspection

The NPR direction is a vehicle for understanding concepts that are universally applicable in real-time rendering: framebuffers, render passes, screen-space algorithms, shader architecture. The creative sensibility comes from a professional background in film lighting and compositing, applied here to real-time graphics.

---

## ✨ Current Features

### Core Setup
- GLFW Wayland window and OpenGL 4.6 core context
- GLAD function pointer loading
- Framebuffer resize handling
- Keyboard input and clean window lifecycle (ESC to exit)

### Mesh Pipeline
- Custom OBJ loader with support for n-gons, missing UVs/normals, and arbitrary face formats
- Interleaved vertex buffer layout (position, normal, UV) uploaded via VAO, VBO, and EBO
- RAII-based Mesh and Shader classes with automatic GPU resource cleanup via destructors
- Vertex and fragment shader pipeline loaded from GLSL files at runtime
- MVP matrix system (model, view, projection) passed as uniforms
- Depth testing for correct fragment ordering

---

## 🗺️ Planned Features

### Layer 1 — Foundation
- Camera system with movement controls
- Phong lighting in GLSL
- Texture loading via stb_image
- Face culling

### Layer 2 — Stylized Film Look
- Deferred rendering pipeline with G-buffer
- Toon shading with configurable light bands
- Hard rim lighting
- Edge detection via normal and depth buffer derivatives
- Limited colour palette via fragment shader quantisation
- G-buffer debug visualisation modes

### Layer 3 — Painterly / Expressive
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

### Shaders
- GLSL vertex and fragment shaders loaded and compiled at runtime
- Shader program linking and error reporting
- Uniform variables for MVP matrix upload

### Math
- Model, View, Projection matrix pipeline
- Perspective projection and look-at view matrix via GLM

### C++ Patterns
- RAII for GPU resource management via constructors and destructors
- Class design for Shader and Mesh
- File parsing with `std::ifstream`, `std::istringstream`, and `std::stringstream`

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

- [LearnOpenGL](https://learnopengl.com/) — primary OpenGL reference
- GLFW and OpenGL documentation

---

## 📁 Project Structure

```
paint-gl/
├── src/                  # C++ source files
│   ├── main.cpp
│   ├── framebuffer.cpp
│   ├── input.cpp
│   ├── obj_loader.cpp
│   └── shader.cpp
├── include/              # Header files
│   ├── framebuffer.h
│   ├── input.h
│   ├── mesh.h
│   ├── obj_loader.h
│   ├── shader.h
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

## 🎬 Demo

*(Coming soon)*
