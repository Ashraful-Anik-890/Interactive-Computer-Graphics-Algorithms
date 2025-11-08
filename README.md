# Interactive Computer Graphics Algorithms

A C implementation of fundamental computer graphics algorithms including Bresenham's Line Drawing, Midpoint Circle Drawing, and 2D Transformations using SDL2.

## Features

- **Bresenham's Line Algorithm (Q2)**: Efficient integer-based line drawing
- **Midpoint Circle Algorithm (Q3)**: Draw circles using 8-way symmetry
- **2D Transformations (Q4)**: Translation, Rotation, and Reflection on triangles
- **Interactive Canvas**: Real-time visualization with coordinate system

## Prerequisites

### Windows

1. **Install MinGW-w64** (for GCC compiler)
   - Download from: https://www.mingw-w64.org/
   - Or use MSYS2: https://www.msys2.org/

2. **Install SDL2**
   - Download SDL2 Development Libraries (MinGW): https://www.libsdl.org/download-2.0.php
   - Extract to a known location (e.g., `C:\SDL2`)

### macOS

```bash
# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install SDL2
brew install sdl2
```

### Linux (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install libsdl2-dev
sudo apt-get install build-essential
```

### Linux (Fedora)

```bash
sudo dnf install SDL2-devel
sudo dnf install gcc make
```

## Installation & Setup in VS Code

### 1. Install VS Code Extensions

- **C/C++** (by Microsoft)
- **Code Runner** (optional, for quick compilation)

### 2. Clone or Download

Save the `graphics_algorithms.c` file to your project directory.

### 3. Configure VS Code (Windows)

Create a `.vscode` folder in your project directory with these files:

**c_cpp_properties.json**:
```json
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/**",
                "C:/SDL2/include"
            ],
            "defines": [],
            "compilerPath": "C:/mingw64/bin/gcc.exe",
            "cStandard": "c11",
            "intelliSenseMode": "gcc-x64"
        }
    ],
    "version": 4
}
```

**tasks.json** (for building):
```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Build Graphics",
            "type": "shell",
            "command": "gcc",
            "args": [
                "-o",
                "graphics_algorithms.exe",
                "graphics_algorithms.c",
                "-IC:/SDL2/include",
                "-LC:/SDL2/lib",
                "-lmingw32",
                "-lSDL2main",
                "-lSDL2",
                "-mwindows"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": ["$gcc"]
        }
    ]
}
```

### 4. Configure VS Code (macOS/Linux)

**tasks.json**:
```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Build Graphics",
            "type": "shell",
            "command": "gcc",
            "args": [
                "-o",
                "graphics_algorithms",
                "graphics_algorithms.c",
                "-lSDL2",
                "-lm"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": ["$gcc"]
        }
    ]
}
```

## Compilation

### Using VS Code

1. Open the project folder in VS Code
2. Press `Ctrl+Shift+B` (or `Cmd+Shift+B` on macOS)
3. Select "Build Graphics" task

### Using Command Line

**Windows (MinGW)**:
```bash
gcc -o graphics_algorithms.exe graphics_algorithms.c -IC:/SDL2/include -LC:/SDL2/lib -lmingw32 -lSDL2main -lSDL2 -mwindows
```

**macOS**:
```bash
gcc -o graphics_algorithms graphics_algorithms.c -lSDL2 -lm
```

**Linux**:
```bash
gcc -o graphics_algorithms graphics_algorithms.c -lSDL2 -lm
```

## Running the Program

### Windows
```bash
./graphics_algorithms.exe
```

### macOS/Linux
```bash
./graphics_algorithms
```

## Usage

Once the program is running, you'll see a window with a coordinate system. Use the following keyboard controls:

| Key | Action |
|-----|--------|
| `1` | Draw Bresenham's Line |
| `2` | Draw Midpoint Circle |
| `3` | Apply 2D Transformations (Triangle) |
| `C` | Clear Canvas |
| `Q` | Quit Program |

### Default Parameters

**Bresenham's Line**:
- Point 1: (10, 12)
- Point 2: (26, 22)
- Color: Green

**Midpoint Circle**:
- Center: (-3, -3)
- Radius: 8
- Color: Yellow

**2D Transformations**:
- Initial Triangle: A(0,0), B(1,1), C(5,2) - Blue
- Transformed Triangle (after translation, rotation, reflection) - Red

## Algorithms Implemented

### 1. Bresenham's Line Algorithm
Efficiently draws lines using only integer arithmetic and addition/subtraction operations. No floating-point calculations required.

### 2. Midpoint Circle Algorithm
Draws circles by calculating points in one octant and using 8-way symmetry to plot all points, minimizing computational overhead.

### 3. 2D Transformations
- **Translation**: Shifts points by (tx, ty)
- **Rotation**: Rotates points around origin by specified angle
- **Reflection**: Mirrors points across the Y-axis

## Troubleshooting

### "SDL2 not found" Error

**Windows**: 
- Ensure SDL2 DLL files are in the same directory as the executable
- Copy `SDL2.dll` from `C:\SDL2\lib` to your project folder

**macOS/Linux**:
- Reinstall SDL2: `brew reinstall sdl2` or `sudo apt-get install --reinstall libsdl2-dev`

### Compilation Errors

- Verify GCC is installed: `gcc --version`
- Check SDL2 installation: `sdl2-config --version` (Linux/macOS)
- Ensure include paths are correct in VS Code configuration

### Window Doesn't Open

- Check if another instance is running
- Verify display/graphics drivers are up to date
- Try running from terminal to see error messages

## Customization

You can modify the default values in the `main()` function:

```c
// Line parameters
int line_x0 = 10, line_y0 = 12, line_x1 = 26, line_y1 = 22;

// Circle parameters
int circle_xc = -3, circle_yc = -3, circle_r = 8;

// Triangle points
int trans_ax = 0, trans_ay = 0;
int trans_bx = 1, trans_by = 1;
int trans_cx = 5, trans_cy = 2;
```

## Project Structure

```
project/
├── graphics_algorithms.c    # Main program
├── README.md               # This file
└── .vscode/               # VS Code configuration (optional)
    ├── c_cpp_properties.json
    └── tasks.json
```

## License

This project is provided as educational material for learning computer graphics algorithms.

## References

- Bresenham's Line Algorithm: J. E. Bresenham (1965)
- Midpoint Circle Algorithm: Based on Bresenham's circle algorithm
- SDL2 Documentation: https://wiki.libsdl.org/
