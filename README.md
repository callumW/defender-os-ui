# defender-os-ui

UI library for Defender OS — C++17, GLFW, OpenGL ES 3.0.

Opens a full-screen OpenGL ES window and drives a unified event & render loop.

---

## Quick start

### Install build dependencies (Debian / Ubuntu / Raspberry Pi OS)

```sh
sudo apt install \
  build-essential cmake pkg-config git \
  libgles2-mesa-dev libegl-dev \
  libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libxext-dev
```

See [`dependencies.md`](dependencies.md) for full details.

### Configure and build

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

### Run

```sh
./build/DefenderOsUI
```

### Run unit tests

```sh
ctest --test-dir build --output-on-failure
```

---

## Project structure

```
src/
  logging/    — Logger (singleton, thread-safe, multi-level)
  core/       — Application, EventLoop
  window/     — IWindow interface + GLFW Window implementation
  renderer/   — IRenderer interface + OpenGL ES Renderer implementation
  platform/   — ApplicationBuilder (production factory)
tests/
  mocks/      — MockWindow, MockRenderer (no GPU required)
  test_*.cpp  — CTest / GoogleTest unit tests
```

---

## Design principles

- **C++17** throughout
- **Smart pointers & RAII** — no raw `new`/`delete`
- **SOLID** — high-level modules depend on `IWindow` / `IRenderer` interfaces
- **Dependency injection** — `Application` accepts pre-built window and renderer, enabling unit testing without a display
- **Unified logging** — all modules use `DefenderOS::Logger` (LOG_INFO / LOG_ERROR / …)
