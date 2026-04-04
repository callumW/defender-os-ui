# Dependencies

This document lists all external dependencies required by `defender-os-ui`, including version constraints, license information, and the rationale for each dependency.

---

## Runtime / Build Dependencies

### GLFW 3.4
- **Purpose**: Cross-platform window creation, OpenGL ES context setup, and input event handling.
- **License**: zlib License (permissive)
- **Source**: https://github.com/glfw/glfw
- **Acquired via**: CMake `FetchContent` (pinned to tag `3.4`)
- **Notes**: Built with `GLFW_BUILD_DOCS`, `GLFW_BUILD_TESTS`, and `GLFW_BUILD_EXAMPLES` disabled to minimise build time.

### OpenGL ES 3.0 (libGLESv2)
- **Purpose**: GPU-accelerated 2D/3D rendering targeting embedded Linux platforms (e.g. Raspberry Pi).
- **License**: Platform-provided (Mesa: MIT; proprietary drivers: vendor-specific)
- **Source**: System package — `libgles2-mesa-dev` or equivalent
- **Install (Debian/Ubuntu)**:
  ```sh
  sudo apt install libgles2-mesa-dev
  ```
- **Notes**: Headers at `<GLES3/gl3.h>`. GLES 3 is a superset of GLES 2; the `glesv2` pkg-config module exposes both.

### EGL (libEGL)
- **Purpose**: Platform-independent interface between OpenGL ES and the native windowing system. GLFW uses EGL to create an OpenGL ES context.
- **License**: Platform-provided (Mesa: MIT; proprietary: vendor-specific)
- **Source**: System package — `libegl-dev` or equivalent
- **Install (Debian/Ubuntu)**:
  ```sh
  sudo apt install libegl-dev
  ```

---

## Test-Only Dependencies

### GoogleTest 1.14.0
- **Purpose**: C++ unit testing framework used with CTest.
- **License**: BSD 3-Clause
- **Source**: https://github.com/google/googletest
- **Acquired via**: CMake `FetchContent` (pinned to tag `v1.14.0`)
- **Notes**: Only downloaded and built when `BUILD_TESTS=ON` (the default).

---

## Build Toolchain Requirements

| Tool | Minimum Version | Notes |
|------|----------------|-------|
| CMake | 3.20 | `FetchContent_MakeAvailable` improvements |
| C++ compiler | GCC 9 / Clang 10 | C++17 required |
| pkg-config | any | Locates system GLES/EGL libraries |
| Git | any | Required by `FetchContent` at configure time |

### Install all build dependencies (Debian/Ubuntu)
```sh
sudo apt install \
  build-essential \
  cmake \
  pkg-config \
  git \
  libgles2-mesa-dev \
  libegl-dev \
  libxrandr-dev \
  libxinerama-dev \
  libxcursor-dev \
  libxi-dev \
  libxext-dev
```

> **Note**: The `libx*-dev` packages are required by GLFW for X11 support on desktop Linux. On a Raspberry Pi with a native framebuffer or Wayland compositor, substitute the appropriate platform-specific GLFW requirements.
