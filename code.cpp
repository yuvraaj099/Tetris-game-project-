#include "raylib.h"
#include <vector>

const int cols = 10, rows = 20;
const int blockSize = 30;
const int screenWidth = cols * blockSize + 200; // Extra space for UI panel
const int screenHeight = rows * blockSize;

// Dark Night Theme Colors
Color backgroundColor = {10, 10, 20, 255}; // Dark blue-black
Color gridColor = {50, 50, 80, 100};       // Subtle grid
Color textColor = {0, 255, 255, 255};      // Neon cyan

// Vector2i for handling positions
struct Vector2i { int x, y; };

// Tetromino shapes
struct Tetromino {
    std::vector<Vector2i> shape;
    Color color;
};

// Tetrominoes with a neon glow effect
Tetromino tetrominoes[7] = {
    {{{0, 0}, {1, 0}, {2, 0}, {3, 0}}, {0, 255, 255, 255}},  // I - Cyan
    {{{0, 0}, {0, 1}, {1, 0}, {1, 1}}, {255, 255, 0, 255}},  // O - Yellow
    {{{0, 0}, {1, 0}, {2, 0}, {1, 1}}, {128, 0, 255, 255}},  // T - Purple
    {{{0, 0}, {1, 0}, {1, 1}, {2, 1}}, {0, 255, 0, 255}},    // S - Green
    {{{1, 0}, {2, 0}, {0, 1}, {1, 1}}, {255, 165, 0, 255}},  // Z - Orange
    {{{0, 0}, {0, 1}, {1, 1}, {2, 1}}, {255, 0, 0, 255}},    // L - Red
    {{{2, 0}, {0, 1}, {1, 1}, {2, 1}}, {0, 191, 255, 255}}   // J - Blue
};

// Grid & Game State
int grid[rows][cols] = {0};
struct Piece {
    Tetromino shape;
    Vector2i pos;

    bool CheckCollision(int dx, int dy) {
        for (auto& block : shape.shape) {
            int x = pos.x + block.x + dx;
            int y = pos.y + block.y + dy;
            if (x < 0 || x >= cols || y >= rows || (y >= 0 && grid[y][x])) return true;
        }
        return false;
    }

    void Move(int dx, int dy) {
        if (!CheckCollision(dx, dy)) pos.x += dx, pos.y += dy;
    }

    void Rotate() {
        std::vector<Vector2i> newShape;
        for (auto& block : shape.shape) newShape.push_back({-block.y, block.x});
        auto oldShape = shape.shape;
        shape.shape = newShape;
        if (CheckCollision(0, 0)) shape.shape = oldShape;
    }

    void Place() {
        for (auto& block : shape.shape) {
            int x = pos.x + block.x, y = pos.y + block.y;
            if (y >= 0) grid[y][x] = 1;
        }
    }
};

Piece currentPiece, nextPiece;
bool gameOver = false;
int score = 0;

void SpawnPiece() {
    currentPiece = nextPiece;
    nextPiece.shape = tetrominoes[GetRandomValue(0, 6)];
    nextPiece.pos = {cols + 1, 5}; // Position for UI preview
    currentPiece.pos = {cols / 2 - 1, 0};
    if (currentPiece.CheckCollision(0, 0)) gameOver = true;
}

void ClearLines() {
    for (int y = rows - 1; y >= 0; y--) {
        bool full = true;
        for (int x = 0; x < cols; x++) {
            if (!grid[y][x]) { full = false; break; }
        }
        if (full) {
            for (int yy = y; yy > 0; yy--) {
                for (int x = 0; x < cols; x++) grid[yy][x] = grid[yy - 1][x];
            }
            for (int x = 0; x < cols; x++) grid[0][x] = 0;
            score += 100;
            y++;
        }
    }
}

void UpdateGame() {
    if (gameOver) return;

    static float fallTimer = 0;
    fallTimer += GetFrameTime();

    if (IsKeyPressed(KEY_LEFT)) currentPiece.Move(-1, 0);
    if (IsKeyPressed(KEY_RIGHT)) currentPiece.Move(1, 0);
    if (IsKeyPressed(KEY_UP)) currentPiece.Rotate();
    if (IsKeyDown(KEY_DOWN)) fallTimer += 0.05f;

    if (fallTimer >= 0.5f) {
        fallTimer = 0;
        if (!currentPiece.CheckCollision(0, 1)) {
            currentPiece.Move(0, 1);
        } else {
            currentPiece.Place();
            ClearLines();
            SpawnPiece();
        }
    }
}

void DrawGame() {
    ClearBackground(backgroundColor);

    // Draw grid
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            DrawRectangleLines(x * blockSize, y * blockSize, blockSize, blockSize, gridColor);
            if (grid[y][x]) DrawRectangle(x * blockSize, y * blockSize, blockSize, blockSize, DARKGRAY);
        }
    }

    // Draw current Tetromino
    for (auto& block : currentPiece.shape.shape) {
        int x = (currentPiece.pos.x + block.x) * blockSize;
        int y = (currentPiece.pos.y + block.y) * blockSize;
        DrawRectangle(x, y, blockSize, blockSize, currentPiece.shape.color);
    }

    // Draw Score & UI
    DrawText("SCORE", screenWidth - 150, 20, 25, textColor);
    DrawText(TextFormat("%d", score), screenWidth - 150, 50, 30, textColor);

    // Draw next piece preview
    DrawText("NEXT", screenWidth - 150, 100, 25, textColor);
    for (auto& block : nextPiece.shape.shape) {
        int x = (cols + block.x) * blockSize;
        int y = (5 + block.y) * blockSize;
        DrawRectangle(x, y, blockSize, blockSize, nextPiece.shape.color);
    }

    if (gameOver) DrawText("GAME OVER - Press ENTER", screenWidth / 4, screenHeight / 2, 20, RED);
}

int main() {
    InitWindow(screenWidth, screenHeight, "Tetris - Dark Night Theme");
    SetTargetFPS(60);
    nextPiece.shape = tetrominoes[GetRandomValue(0, 6)];
    SpawnPiece();

    while (!WindowShouldClose()) {
        if (gameOver && IsKeyPressed(KEY_ENTER)) {
            gameOver = false; score = 0;
            for (auto& row : grid) for (int& cell : row) cell = 0;
            SpawnPiece();
        }
        UpdateGame();
        BeginDrawing();
        DrawGame();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
