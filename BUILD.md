# Building DefenderOS UI Library

## Prerequisites

Before building, you need to install the required dependencies.

### Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install -y \
    cmake \
    g++ \
    libsdl2-dev \
    libsdl2-ttf-dev \
    git
```

### Fedora/RHEL/CentOS

```bash
sudo dnf install -y \
    cmake \
    gcc-c++ \
    SDL2-devel \
    SDL2_ttf-devel \
    git
```

### Arch Linux

```bash
sudo pacman -S cmake gcc sdl2 sdl2_ttf git
```

### macOS

```bash
brew install cmake sdl2 sdl2_ttf
```

### Buildroot

For embedded systems using Buildroot, add to your `defconfig`:

```
BR2_PACKAGE_SDL2=y
BR2_PACKAGE_SDL2_TTF=y
```

## Building

### Standard Build

```bash
# Clone the repository (if not already done)
git clone https://github.com/callumW/defender-os-ui.git
cd defender-os-ui

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build
make -j$(nproc)
```

### Build Options

You can control what gets built:

```bash
# Build without examples
cmake -DBUILD_EXAMPLES=OFF ..

# Build without tests
cmake -DBUILD_TESTS=OFF ..

# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build with optimizations
cmake -DCMAKE_BUILD_TYPE=Release ..
```

### Cross-Compilation

For cross-compilation (e.g., for Buildroot), create a toolchain file:

```cmake
# toolchain.cmake
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER /path/to/arm-linux-gcc)
set(CMAKE_CXX_COMPILER /path/to/arm-linux-g++)

set(CMAKE_FIND_ROOT_PATH /path/to/sysroot)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
```

Then build with:

```bash
cmake -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake ..
make
```

## Testing

After building, run the test suite:

```bash
cd build
ctest --output-on-failure
```

Or run individual tests:

```bash
./tests/test_logger
./tests/test_ui_element
./tests/test_theme
./tests/test_input_state
```

## Running Examples

The examples demonstrate the library features:

```bash
# Simple button example
./examples/simple_example

# Menu system example (like in-car navigation)
./examples/menu_example
```

**Note**: Examples require a display server (X11, Wayland, etc.). For headless systems, you may need to configure SDL2 to use a dummy video driver:

```bash
SDL_VIDEODRIVER=dummy ./examples/simple_example
```

## Installing

To install the library system-wide:

```bash
cd build
sudo make install
```

This will install:
- Headers to `/usr/local/include/defender/`
- Library to `/usr/local/lib/`

## Troubleshooting

### SDL2 Not Found

If CMake can't find SDL2:

1. Check if SDL2 is installed:
   ```bash
   pkg-config --modversion sdl2
   ```

2. If installed but not found, set PKG_CONFIG_PATH:
   ```bash
   export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH
   ```

### Missing Fonts

If you get font-related errors:

1. The default theme uses DejaVu fonts. Install them:
   ```bash
   # Ubuntu/Debian
   sudo apt-get install fonts-dejavu
   
   # Fedora/RHEL
   sudo dnf install dejavu-sans-fonts
   ```

2. Or update the theme to use fonts available on your system.

### Display Issues

If examples don't show a window:

1. Check DISPLAY variable:
   ```bash
   echo $DISPLAY
   ```

2. For WSL or remote systems, you may need an X server (VcXsrv, Xming, etc.)

3. For headless testing:
   ```bash
   SDL_VIDEODRIVER=dummy ./examples/simple_example
   ```

## IDE Support

### Visual Studio Code

The build generates `compile_commands.json` for IDE support. Install the C++ extension and it should work automatically.

### CLion

Open the project directory and CLion will automatically detect the CMake configuration.

## Cleaning

To clean build artifacts:

```bash
cd build
make clean
```

Or delete the entire build directory:

```bash
rm -rf build
```
