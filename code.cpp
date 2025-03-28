#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>
#include <unistd.h>

const int WIDTH = 10, HEIGHT = 20;

struct Vector2 { int x, y; };

struct Tetromino {
    std::vector<Vector2> shape;
    Vector2 pos;
    int color;
};

Tetromino tetrominoes[7] = {
    {{{0, 0}, {1, 0}, {2, 0}, {3, 0}}, {3, 0}, COLOR_CYAN},   // I
    {{{0, 0}, {0, 1}, {1, 0}, {1, 1}}, {4, 0}, COLOR_YELLOW}, // O
    {{{0, 0}, {1, 0}, {2, 0}, {1, 1}}, {4, 0}, COLOR_MAGENTA},// T
    {{{0, 0}, {1, 0}, {1, 1}, {2, 1}}, {4, 0}, COLOR_GREEN},  // S
    {{{1, 0}, {2, 0}, {0, 1}, {1, 1}}, {4, 0}, COLOR_RED},    // Z
    {{{0, 0}, {0, 1}, {0, 2}, {1, 2}}, {4, 0}, COLOR_BLUE},   // L
    {{{1, 0}, {1, 1}, {1, 2}, {0, 2}}, {4, 0}, COLOR_WHITE}   // J
};

int grid[HEIGHT][WIDTH] = {0};
Tetromino current, nextTetromino;
bool gameOver = false;
int score = 0;
int level = 1;
int fallSpeed = 500000;
time_t lastDropTime;

void InitGame() {
    srand(time(0));
    initscr();
    start_color();
    noecho();
    curs_set(0);
    timeout(1);

    for (int i = 1; i <= 7; i++) {
        init_pair(i, i, COLOR_BLACK);
    }
    init_pair(8, COLOR_RED, COLOR_BLACK);

    current = tetrominoes[rand() % 7];
    current.pos = {3, 0};
    nextTetromino = tetrominoes[rand() % 7]; // Initialize the next piece
    lastDropTime = time(0);
}

bool Collision(int dx, int dy) {
    for (auto& block : current.shape) {
        int x = current.pos.x + block.x + dx;
        int y = current.pos.y + block.y + dy;
        if (x < 0 || x >= WIDTH || y >= HEIGHT || (y >= 0 && grid[y][x])) return true;
    }
    return false;
}

void MovePiece(int dx, int dy) {
    if (!Collision(dx, dy)) current.pos.x += dx, current.pos.y += dy;
}

void HardDrop() {
    while (!Collision(0, 1)) MovePiece(0, 1);
}

void RotatePiece() {
    std::vector<Vector2> rotated;
    for (auto& block : current.shape) {
        rotated.push_back({-block.y, block.x});
    }
    Tetromino rotatedTetromino = {rotated, current.pos, current.color};
    bool canRotate = true;
    for (auto& block : rotated) {
        int x = rotatedTetromino.pos.x + block.x;
        int y = rotatedTetromino.pos.y + block.y;
        if (x < 0 || x >= WIDTH || y >= HEIGHT || (y >= 0 && grid[y][x])) {
            canRotate = false;
            break;
        }
    }
    if (canRotate) current.shape = rotated;
}

void PlacePiece() {
    for (auto& block : current.shape) {
        int x = current.pos.x + block.x;
        int y = current.pos.y + block.y;
        if (y >= 0) grid[y][x] = current.color;
    }
}

void ClearLines() {
    for (int y = HEIGHT - 1; y >= 0; y--) {
        bool full = true;
        for (int x = 0; x < WIDTH; x++) {
            if (!grid[y][x]) full = false;
        }
        if (full) {
            for (int yy = y; yy > 0; yy--) {
                for (int x = 0; x < WIDTH; x++) {
                    grid[yy][x] = grid[yy - 1][x];
                }
            }
            for (int x = 0; x < WIDTH; x++) grid[0][x] = 0;
            score += 10;
            y++;
        }
    }
}

void SpawnPiece() {
    current = nextTetromino;  // Use the stored next tetromino
    current.pos = {3, 0};
    nextTetromino = tetrominoes[rand() % 7]; // Generate a new next piece

    if (Collision(0, 0)) gameOver = true;
}

void DrawNextTetromino() {
    mvprintw(3, WIDTH * 2 + 2, "Next:");
    attron(COLOR_PAIR(nextTetromino.color));
    for (auto& block : nextTetromino.shape) {
        int x = WIDTH * 2 + 5 + block.x * 2; // Offset to the right
        int y = 5 + block.y;                 // Place below the "Next" text
        mvprintw(y, x, "[]");
    }
    attroff(COLOR_PAIR(nextTetromino.color));
}

void DrawGame() {
    clear();
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (grid[y][x]) {
                attron(COLOR_PAIR(grid[y][x]));
                mvprintw(y, x * 2, "[]");
                attroff(COLOR_PAIR(grid[y][x]));
            } else {
                mvprintw(y, x * 2, " .");
            }
        }
    }

    attron(COLOR_PAIR(current.color));
    for (auto& block : current.shape) {
        int x = current.pos.x + block.x;
        int y = current.pos.y + block.y;
        if (y >= 0) mvprintw(y, x * 2, "[]");
    }
    attroff(COLOR_PAIR(current.color));

    mvprintw(0, WIDTH * 2 + 2, "Score: %d", score);
    mvprintw(1, WIDTH * 2 + 2, "Level: %d", level);

    DrawNextTetromino();  // Display next tetromino

    if (gameOver) {
        attron(COLOR_PAIR(8));
        mvprintw(10, 5, "GAME OVER!");
        attroff(COLOR_PAIR(8));
    }
    refresh();
}

void UpdateGame() {
    int ch = getch();
    if (ch == 'a') MovePiece(-1, 0);
    if (ch == 'd') MovePiece(1, 0);
    if (ch == 's') MovePiece(0, 1);
    if (ch == 'w') RotatePiece();
    if (ch == ' ') HardDrop();
    if (ch == 'q') gameOver = true;

    time_t currentTime = time(0);
    if (currentTime - lastDropTime >= 1) {
        if (!Collision(0, 1)) {
            MovePiece(0, 1);
        } else {
            PlacePiece();
            ClearLines();
            SpawnPiece();
        }
        lastDropTime = currentTime;
    }
}

int main() {
    InitGame();
    while (!gameOver) {
        UpdateGame();
        DrawGame();
        usleep(50000);
    }
    endwin();
    std::cout << "Game Over! Final Score: " << score << "\n";
    return 0;
}
