## GPU Fluid Simulation
<img width="1920" height="1080" alt="screenshot-2025-11-14_18:48:50" src="https://github.com/user-attachments/assets/ea006291-3303-46b7-a9e4-30e7563b122c" />
Real-time 2D fluid simulation created in C++ and GLSL Shaders (OpenGL)

### Installation Guide
##### Dependencies:
- GLFW3
- Build dependencies:
  - CMake
  - make
  - g++
##### Steps:
- Clone the repo and cd into it
- `mkdir build && cd build`
- `cmake .. && make -j$(nproc)`
- Builds binary `fluidsim` inside the build directory

### How it works
- Ping-pong texture pipeline
- Uses fragment shaders to simulate fluid physics
  - Computes divergence field from the velocity field
  - Uses jacobi pressure solve to estimate pressure field
  - Computes the pressure gradient
  - Update velocity field based on pressure gradient to make fluid incompressible
  - Advect velocity field
  - Advect color field

Achieves 60-100 fps on Intel Integrated Graphics

Thanks to learnopengl.com for learning the basics of OpenGL textures, framebuffers, and GLSL
