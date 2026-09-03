#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define WIDTH   20   // 每层宽度
#define HEIGHT  15   // 每层高度
#define DEPTH    3   // 层数
#define MAX_SNAKE 500

typedef struct {
    int x, y, z;
} Point3D;

// 全局变量
Point3D snake[MAX_SNAKE];
int snake_len;
Point3D food;
// direction: 0=上(Y-) 1=右(X+) 2=下(Y+) 3=左(X-) 4=上层(Z-) 5=下层(Z+)
int direction;
int score;
int game_over;
int paused;
int speed;       // 移动间隔(ms)
int speed_level; // 速度等级 1~10

// 隐藏光标
void hide_cursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(hOut, &info);
    info.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &info);
}

void gotoxy(int x, int y) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, pos);
}

void set_color(int fg, int bg) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, (WORD)((bg << 4) | fg));
}

void center_window() {
    HWND hwnd = GetConsoleWindow();
    if (hwnd == NULL) return;
    RECT rect;
    GetWindowRect(hwnd, &rect);
    int ww = rect.right - rect.left;
    int wh = rect.bottom - rect.top;
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    SetWindowPos(hwnd, HWND_TOP, (sw - ww) / 2, (sh - wh) / 2, 0, 0, SWP_NOSIZE);
}

// 检查3D位置是否可放置
int is_valid_3d(int x, int y, int z) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT || z < 0 || z >= DEPTH)
        return 0;
    if (x == food.x && y == food.y && z == food.z) return 0;
    for (int i = 0; i < snake_len; i++) {
        if (snake[i].x == x && snake[i].y == y && snake[i].z == z)
            return 0;
    }
    return 1;
}

void init_game() {
    snake_len = 3;
    // 蛇从第1层中间水平排列
    int mid_x = WIDTH / 2;
    int mid_y = HEIGHT / 2;
    int mid_z = DEPTH / 2;
    for (int i = 0; i < snake_len; i++) {
        snake[i].x = mid_x - i;
        snake[i].y = mid_y;
        snake[i].z = mid_z;
    }
    direction = 1;  // 初始向右
    score = 0;
    game_over = 0;
    paused = 0;
    speed = 180;
    speed_level = 1;

    srand((unsigned)time(NULL));
    // 随机生成食物
    do {
        food.x = rand() % WIDTH;
        food.y = rand() % HEIGHT;
        food.z = rand() % DEPTH;
    } while (!is_valid_3d(food.x, food.y, food.z));
}

// 绘制3D游戏画面（三层并排显示）
void draw() {
    gotoxy(0, 0);

    // 层标签
    set_color(11, 0);
    for (int z = 0; z < DEPTH; z++) {
        printf("  ┌");
        for (int i = 0; i < WIDTH; i++) printf("─");
        printf("┐  ");
    }
    printf("\n");

    for (int z = 0; z < DEPTH; z++) {
        set_color(11, 0);
        printf("  │ 第%d层", z + 1);
        for (int i = 0; i < WIDTH - 8; i++) printf(" ");
        printf("│  ");
    }
    printf("\n");

    for (int z = 0; z < DEPTH; z++) {
        set_color(11, 0);
        printf("  ├");
        for (int i = 0; i < WIDTH; i++) printf("─");
        printf("┤  ");
    }
    printf("\n");

    // 绘制每层内容（行优先）
    for (int y = 0; y < HEIGHT; y++) {
        for (int z = 0; z < DEPTH; z++) {
            set_color(11, 0);
            printf("  │");
            for (int x = 0; x < WIDTH; x++) {
                int is_snake = 0, is_head = 0;
                for (int i = 0; i < snake_len; i++) {
                    if (snake[i].x == x && snake[i].y == y && snake[i].z == z) {
                        is_snake = 1;
                        if (i == 0) is_head = 1;
                        break;
                    }
                }
                int is_food = (food.x == x && food.y == y && food.z == z);

                if (is_head) {
                    set_color(10, 0);
                    char hc = 'O';
                    switch (direction) {
                        case 0: hc = '^'; break;
                        case 1: hc = '>'; break;
                        case 2: hc = 'v'; break;
                        case 3: hc = '<'; break;
                        case 4: hc = 'U'; break;  // 上层
                        case 5: hc = 'D'; break;  // 下层
                    }
                    printf("%c", hc);
                } else if (is_snake) {
                    set_color(2, 0);
                    printf("o");
                } else if (is_food) {
                    set_color(12, 0);
                    printf("♥");
                } else {
                    set_color(8, 0);
                    printf("·");
                }
            }
            set_color(11, 0);
            printf("│  ");
        }
        printf("\n");
    }

    // 底部分隔线和状态栏
    for (int z = 0; z < DEPTH; z++) {
        set_color(11, 0);
        printf("  └");
        for (int i = 0; i < WIDTH; i++) printf("─");
        printf("┘  ");
    }
    printf("\n");

    // 状态栏
    set_color(14, 0);
    printf("\n  🏆 分数: %d  |  速度: Lv.%d", score, speed_level);
    set_color(8, 0);
    printf(" [");
    for (int i = 1; i <= 10; i++) {
        if (i <= speed_level) {
            if (speed_level <= 3) set_color(10, 0);
            else if (speed_level <= 6) set_color(14, 0);
            else set_color(12, 0);
            printf("■");
        } else {
            set_color(8, 0);
            printf("□");
        }
    }
    set_color(8, 0); printf("]");
    set_color(14, 0);
    printf("  |  WASD/方向键 XY移动  |  Q/E 切换层  |  P 暂停\n");
    set_color(7, 0);

    if (paused) {
        int cx = (DEPTH * (WIDTH + 4) - 14) / 2;
        int cy = HEIGHT / 2 + 4;
        gotoxy(cx, cy - 1);
        set_color(0, 14);
        printf("                  ");
        gotoxy(cx, cy);
        printf("  ⏸  游 戏 暂 停  ");
        gotoxy(cx, cy + 1);
        printf("  按 P 键继续...   ");
        gotoxy(cx, cy + 2);
        printf("                  ");
        set_color(7, 0);
    }
}

void spawn_food() {
    int attempts = 0;
    do {
        food.x = rand() % WIDTH;
        food.y = rand() % HEIGHT;
        food.z = rand() % DEPTH;
        attempts++;
    } while (!is_valid_3d(food.x, food.y, food.z) && attempts < 500);
}

void update() {
    Point3D new_head = snake[0];
    switch (direction) {
        case 0: new_head.y--; break;
        case 1: new_head.x++; break;
        case 2: new_head.y++; break;
        case 3: new_head.x--; break;
        case 4: new_head.z--; break;
        case 5: new_head.z++; break;
    }

    // 边界检查
    if (new_head.x < 0 || new_head.x >= WIDTH ||
        new_head.y < 0 || new_head.y >= HEIGHT ||
        new_head.z < 0 || new_head.z >= DEPTH) {
        game_over = 1;
        return;
    }

    // 自碰检查
    for (int i = 0; i < snake_len - 1; i++) {
        if (snake[i].x == new_head.x &&
            snake[i].y == new_head.y &&
            snake[i].z == new_head.z) {
            game_over = 1;
            return;
        }
    }

    // 移动蛇
    for (int i = snake_len; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    snake[0] = new_head;

    // 吃食物
    if (snake[0].x == food.x && snake[0].y == food.y && snake[0].z == food.z) {
        snake_len++;
        score += 10;
        spawn_food();
        // 加速
        if (speed > 50) {
            speed -= 5;
            if (speed < 50) speed = 50;
        }
        speed_level = 1 + (180 - speed) * 9 / 130;
        if (speed_level > 10) speed_level = 10;
    }
}

void input() {
    if (_kbhit()) {
        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (paused) return;
            switch (key) {
                case 72: if (direction != 2) direction = 0; break;
                case 80: if (direction != 0) direction = 2; break;
                case 75: if (direction != 1) direction = 3; break;
                case 77: if (direction != 3) direction = 1; break;
            }
        } else {
            if (key == 'p' || key == 'P') { paused = !paused; return; }
            if (paused) {
                if (key == 'q' || key == 'Q') game_over = 1;
                return;
            }
            switch (key) {
                case 'w': case 'W': if (direction != 2) direction = 0; break;
                case 's': case 'S': if (direction != 0) direction = 2; break;
                case 'a': case 'A': if (direction != 1) direction = 3; break;
                case 'd': case 'D': if (direction != 3) direction = 1; break;
                case 'q': case 'Q': if (direction != 5) direction = 4; break;
                case 'e': case 'E': if (direction != 4) direction = 5; break;
            }
        }
    }
}

int main() {
    system("chcp 65001 >nul");
    system("cls");
    center_window();

    set_color(11, 0);
    printf("\n\n");
    printf("  ╔══════════════════════════════════════════╗\n");
    printf("  ║                                          ║\n");
    printf("  ║       🐍  3D 贪 吃 蛇  🐍              ║\n");
    printf("  ║                                          ║\n");
    printf("  ╚══════════════════════════════════════════╝\n\n");

    set_color(14, 0);
    printf("  🎮 操作说明:\n\n");
    set_color(7, 0);
    printf("    W/↑上  S/↓下  A/←左  D/→右  ——  XY平面移动\n");
    printf("    Q      ——  切换到上一层（Z-1）\n");
    printf("    E      ——  切换到下一层（Z+1）\n");
    printf("    P      ——  暂停\n");
    printf("    Q(暂停时) —— 退出\n\n");

    set_color(14, 0);
    printf("  📐 游戏空间: %d × %d × %d (宽×高×深)\n\n", WIDTH, HEIGHT, DEPTH);
    set_color(7, 0);
    printf("  按任意键开始...");
    _getch();

    int restart;
    do {
        restart = 0;
        hide_cursor();
        system("cls");
        init_game();

        while (!game_over) {
            draw();
            input();
            if (!paused) update();
            for (int i = 0; i < speed / 10; i++) {
                input();
                if (game_over) break;
                if (paused) { i--; continue; }
                Sleep(10);
            }
        }

        // 游戏结束
        system("cls");
        set_color(12, 0);
        printf("\n\n");
        printf("  ╔═══════════════════════════════╗\n");
        printf("  ║                               ║\n");
        printf("  ║       游  戏  结  束  !       ║\n");
        printf("  ║                               ║\n");
        printf("  ╚═══════════════════════════════╝\n");
        set_color(14, 0);
        printf("\n         🏆 最终得分: %d\n\n", score);
        set_color(7, 0);
        printf("    按 R 重新开始  |  按 Q 退出\n");

        while (1) {
            if (_kbhit()) {
                int key = _getch();
                if (key == 'r' || key == 'R') { restart = 1; break; }
                if (key == 'q' || key == 'Q') break;
            }
        }
        system("cls");
    } while (restart);

    set_color(7, 0);
    printf("\n  感谢游玩！再见！👋\n\n");
    return 0;
}