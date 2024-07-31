#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <ctime>

const int WIDTH = 4;  // Width of the grid
int grid[WIDTH][WIDTH];  // Game grid
int score = 0;

void drawTile(int x, int y, int value) {
    int color;
    switch (value) {
        case 2: color = CYAN; break;
        case 4: color = YELLOW; break;
        case 8: color = LIGHTRED; break;
        case 16: color = LIGHTMAGENTA; break;
        case 32: color = LIGHTGREEN; break;
        case 64: color = LIGHTCYAN; break;
        case 128: color = GREEN; break;
        case 256: color = LIGHTBLUE; break;
        case 512: color = BLUE; break;
        case 1024: color = MAGENTA; break;
        case 2048: color = RED; break;
        default: color = LIGHTGRAY; break;
    }
    setfillstyle(SOLID_FILL, color);
    bar(x, y, x + 100, y + 100);
    if (value != 0) {
        char str[5];
        sprintf(str, "%d", value);
        setcolor(WHITE);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
        outtextxy(x + 50 - textwidth(str) / 2, y + 50 - textheight(str) / 2, str);
    }
}

void drawGrid() {
    cleardevice();
    //Title
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 6);
    outtextxy(80, 30, "Game 2048");
    //Score
    char scoreText[20];
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    sprintf(scoreText, "Score:%d", score);
    outtextxy(70, 140, scoreText);
	//Gameplay
    setfillstyle(SOLID_FILL, BLACK);
    bar(70, 190, 520, 640); // Background
    setcolor(WHITE);
    rectangle(70, 190, 520, 640); // Border
    // Draw the individual tiles
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            drawTile(j * 110 + 80, i * 110 + 200, grid[i][j]); // Adjusted position
        }
    }
    // NewGame
    setcolor(WHITE);
    rectangle(340, 130, 550, 170);
    outtextxy(350, 138, "New Game");
}

void spawnTile() {
    int x, y;
    do {
        x = rand() % WIDTH;
        y = rand() % WIDTH;
    } while (grid[x][y] != 0);
    grid[x][y] = (rand() % 2 + 1) * 2;
}

bool canMove() {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (grid[i][j] == 0) return true;
            if (i < WIDTH - 1 && grid[i][j] == grid[i + 1][j]) return true;
            if (j < WIDTH - 1 && grid[i][j] == grid[i][j + 1]) return true;
        }
    }
    return false;
}

bool moveLeft() {
    bool moved = false;
    for (int i = 0; i < WIDTH; i++) {
        int lastMergePos = -1;
        for (int j = 1; j < WIDTH; j++) {
            if (grid[i][j] == 0) continue;
            int k = j;
            while (k > 0 && grid[i][k - 1] == 0) {
                grid[i][k - 1] = grid[i][k];
                grid[i][k] = 0;
                k--;
                moved = true;
            }
            if (k > 0 && grid[i][k - 1] == grid[i][k] && lastMergePos != k - 1) {
                grid[i][k - 1] *= 2;
                grid[i][k] = 0;
                lastMergePos = k - 1;
                score += grid[i][k - 1];  // Increase score
                moved = true;
            }
        }
    }
    return moved;
}

bool moveRight() {
    bool moved = false;
    for (int i = 0; i < WIDTH; i++) {
        int lastMergePos = WIDTH;
        for (int j = WIDTH - 2; j >= 0; j--) {
            if (grid[i][j] == 0) continue;
            int k = j;
            while (k < WIDTH - 1 && grid[i][k + 1] == 0) {
                grid[i][k + 1] = grid[i][k];
                grid[i][k] = 0;
                k++;
                moved = true;
            }
            if (k < WIDTH - 1 && grid[i][k + 1] == grid[i][k] && lastMergePos != k + 1) {
                grid[i][k + 1] *= 2;
                grid[i][k] = 0;
                lastMergePos = k + 1;
                score += grid[i][k + 1];  // Increase score
                moved = true;
            }
        }
    }
    return moved;
}

bool moveUp() {
    bool moved = false;
    for (int j = 0; j < WIDTH; j++) {
        int lastMergePos = -1;
        for (int i = 1; i < WIDTH; i++) {
            if (grid[i][j] == 0) continue;
            int k = i;
            while (k > 0 && grid[k - 1][j] == 0) {
                grid[k - 1][j] = grid[k][j];
                grid[k][j] = 0;
                k--;
                moved = true;
            }
            if (k > 0 && grid[k - 1][j] == grid[k][j] && lastMergePos != k - 1) {
                grid[k - 1][j] *= 2;
                grid[k][j] = 0;
                lastMergePos = k - 1;
                score += grid[k - 1][j];  // Increase score
                moved = true;
            }
        }
    }
    return moved;
}

bool moveDown() {
    bool moved = false;
    for (int j = 0; j < WIDTH; j++) {
        int lastMergePos = WIDTH;
        for (int i = WIDTH - 2; i >= 0; i--) {
            if (grid[i][j] == 0) continue;
            int k = i;
            while (k < WIDTH - 1 && grid[k + 1][j] == 0) {
                grid[k + 1][j] = grid[k][j];
                grid[k][j] = 0;
                k++;
                moved = true;
            }
            if (k < WIDTH - 1 && grid[k + 1][j] == grid[k][j] && lastMergePos != k + 1) {
                grid[k + 1][j] *= 2;
                grid[k][j] = 0;
                lastMergePos = k + 1;
                score += grid[k + 1][j];  // Increase score
                moved = true;
            }
        }
    }
    return moved;
}

void initGame() {
    srand(time(0));
    memset(grid, 0, sizeof(grid));
    score = 0;  // Reset score
    for (int i = 0; i < 2; i++) {
        spawnTile();
    }
}

bool isNewGameClicked(int x, int y) {
    return (x >= 340 && x <= 550 && y >= 130 && y <= 170);
}

int main() {
    initwindow(600, 700);
    bool gameOver = false;
    initGame();
    drawGrid();
    while (true) {
        if (kbhit() && !gameOver) {
            char key = getch();
            bool moved = false;
            switch (key) {
                case 'a': moved = moveLeft(); break;
                case 'd': moved = moveRight(); break;
                case 'w': moved = moveUp(); break;
                case 's': moved = moveDown(); break;
            }
            if (moved) {
                spawnTile();
                drawGrid();
                if (!canMove()) {
                    gameOver = true;
                    setcolor(WHITE);
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
                    setfillstyle(SOLID_FILL, BLACK);
                    bar(72, 390, 518, 440);
                    outtextxy(150, 400, "Game Over!");
                }
            }
        }
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if (isNewGameClicked(x, y)) {
                gameOver = false;
                initGame();
                drawGrid();
            }
        }
    }
    getch();
    closegraph();
    return 0;
}

