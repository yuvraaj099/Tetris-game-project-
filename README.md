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
- Intuitive UI for easy gameplay
- Lightweight and fast performance
- Grid-based system for block placement
- Collision detection for movement and rotation
- Line clearing and score updating mechanism
- Randomized tetromino spawning
- Responsive keyboard controls
- Simple yet visually appealing design

## Controls
- **Left Arrow (←)**: Move piece left
- **Right Arrow (→)**: Move piece right
- **Up Arrow (↑)**: Rotate piece
- **Down Arrow (↓)**: Move piece down faster
- **Enter (⏎)**: Restart game after game over
- **Escape (Esc)**: Exit the game instantly

## Installation & Compilation
### Prerequisites
- **raylib** installed on your system
- A C++ compiler (MinGW-w64, GCC, Clang, or MSVC)
- Basic knowledge of compiling C++ programs

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

## Code Overview
### Game Grid & State Management
- The game consists of a **10x20 grid**, where each cell stores block data.
- A **2D array** (`grid[rows][cols]`) keeps track of placed blocks.
- The game state updates at a regular interval to handle falling pieces.

### Tetrominoes & Movement
- Each tetromino is stored as a **vector of relative block positions**.
- The **`Piece` structure** holds the current tetromino and its position.
- **Collision detection** ensures blocks do not overlap or move out of bounds.
- Pieces can **move left, right, down, or rotate** if space allows.
- The game includes a function to **rotate tetrominoes** while handling wall collisions.

### Line Clearing & Scoring
- When a row is **completely filled**, it is cleared and blocks above it shift down.
- Score is increased by **100 points per cleared line**.
- Multiple cleared lines in a row result in bonus points.

### Random Tetromino Selection
- Pieces are randomly selected from a predefined **array of tetrominoes**.
- A **preview piece** is displayed to show the next tetromino.
- The game ensures balanced distribution of different tetrominoes.

## Dependencies
- **raylib**: https://www.raylib.com

## License
This project is open-source under the **MIT License**. Feel free to modify and distribute it!

## Future Enhancements
- Adding background music and sound effects
- Implementing different game modes and difficulty levels
- Enhancing animations and visual effects
- Implementing a leaderboard for high scores
- Adding multiplayer functionality
- Saving high scores and game progress
- Implementing custom themes and color schemes

## Screenshots
*(Add screenshots here if needed)*

