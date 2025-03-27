# Tetris - Dark Night Theme

## Description
This is a simple Tetris game built using the **raylib** library. It features a **dark night theme** with neon-colored tetrominoes and a minimalistic UI.

## Features
- Classic Tetris gameplay
- Dark night theme with neon colors
- Score tracking
- Next piece preview
- Smooth block movement and rotation
- Game over detection

## Controls
- **Left Arrow (←)**: Move piece left
- **Right Arrow (→)**: Move piece right
- **Up Arrow (↑)**: Rotate piece
- **Down Arrow (↓)**: Move piece down faster
- **Enter (⏎)**: Restart game after game over

## Installation & Compilation
### Prerequisites
- **raylib** installed on your system
- A C++ compiler (MinGW-w64, GCC, Clang, or MSVC)

### Compilation (Windows - MinGW)
```sh
g++ -o tetris tetris.cpp -O2 -Wall -std=c++17 -lraylib -lopengl32 -lgdi32 -lwinmm
```

### Compilation (Linux)
```sh
g++ -o tetris tetris.cpp -O2 -Wall -std=c++17 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

### Compilation (macOS)
```sh
clang++ -o tetris tetris.cpp -O2 -Wall -std=c++17 -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
```

## Running the Game
After compilation, run the executable:
```sh
./tetris
```

## Dependencies
- **raylib**: https://www.raylib.com

## License
This project is open-source under the **MIT License**. Feel free to modify and distribute it!

## Screenshots
*(Add screenshots here if needed)*

