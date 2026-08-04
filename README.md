# Raytracer

A learning project following the **Ray Tracing in One Weekend** book series by Peter Shirley.

## Book Series

- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
- [Ray Tracing: The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
- [Ray Tracing: The Rest of Your Life](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)
- [Series Home Page](https://raytracing.github.io/)

## Project Goals

Implement a simple but functional ray tracer in modern C++ while learning the
core algorithms from the book:

1. Output images in PPM format
2. Vectors, rays, and the camera
3. Ray-sphere intersection
4. Surface scattering and materials
5. Anti-aliasing, depth of field, and defocus blur

## Build

This project uses **CMake** with the **C++20** standard.

### Command Line

```bash
cmake -S . -B build
cmake --build build --config Debug
```

Run the program (output appears in `build/bin`):

```bash
./build/bin/Debug/Raytracer        # MSVC / Visual Studio generator
./build/bin/Raytracer              # Ninja / MinGW Makefiles generator
```

### IDE (Trae / VS Code)

Recommended extensions:

- **C/C++** (`ms-vscode.cpptools`)
- **CMake Tools** (`ms-vscode.cmake-tools`)

Build tasks are predefined in `.vscode/tasks.json` (`Ctrl+Shift+B`).
Debug configurations are predefined in `.vscode/launch.json` (`F5`):

- `(MSVC) Debug Raytracer` - when using the Visual Studio compiler.
- `(MinGW/gdb) Debug Raytracer` - when using MinGW/GCC.

## Project Layout

```
Raytracer/
├── CMakeLists.txt
├── README.md
├── src/
│   └── main.cpp
└── .vscode/
    ├── launch.json
    ├── tasks.json
    └── settings.json
```

## Progress

- [x] Project scaffold (CMake + C++20 Hello World)
- [ ] Chapter 1: Output an image
- [ ] Chapter 2: The vec3 class
- [ ] Chapter 3: Rays, a simple camera, and background
- [ ] Chapter 4: Adding a sphere
- [ ] Chapter 5: Surface normals and multiple objects
- [ ] Chapter 6: Antialiasing
- [ ] Chapter 7: Diffuse materials
- [ ] Chapter 8: Metal
- [ ] Chapter 9: Dielectrics
- [ ] Chapter 10: Positionable camera
- [ ] Chapter 11: Defocus blur
- [ ] Chapter 12: Where next?
