# Dependencies

## Build Dependencies

### Required
- **CMake** (>= 3.15): Build system generator
- **C++ Compiler**: Supporting C++17 standard (GCC 7+, Clang 5+, MSVC 2017+)
- **SDL2** (>= 2.0.0): Simple DirectMedia Layer for graphics and input
- **SDL2_ttf** (>= 2.0.0): TrueType font rendering support for SDL2

### Optional
- **CTest**: Unit testing framework (included with CMake)

## Runtime Dependencies

- **SDL2** (>= 2.0.0): Runtime library for graphics
- **SDL2_ttf** (>= 2.0.0): Runtime library for font rendering
- **Font files**: TrueType fonts for text rendering

## Installation

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install -y cmake g++ libsdl2-dev libsdl2-ttf-dev
```

### Fedora/RHEL
```bash
sudo dnf install cmake gcc-c++ SDL2-devel SDL2_ttf-devel
```

### Buildroot
Add the following packages to your buildroot configuration:
- `BR2_PACKAGE_SDL2=y`
- `BR2_PACKAGE_SDL2_TTF=y`

In your buildroot `defconfig` or `.config`:
```
BR2_PACKAGE_SDL2=y
BR2_PACKAGE_SDL2_TTF=y
```

## Development Tools

- **Git**: Version control
- **GDB**: Debugging (optional)
- **Valgrind**: Memory leak detection (optional)

## Design Principles

This library follows:
- **RAII** (Resource Acquisition Is Initialization): All resources are managed through object lifetimes
- **Smart Pointers**: Using `std::unique_ptr` and `std::shared_ptr` for memory management
- **SOLID Principles**:
  - Single Responsibility: Each class has one reason to change
  - Open/Closed: Open for extension, closed for modification
  - Liskov Substitution: Derived classes are substitutable for base classes
  - Interface Segregation: Clients shouldn't depend on unused interfaces
  - Dependency Inversion: Depend on abstractions, not concretions
