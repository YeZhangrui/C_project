#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define WIDTH  50   // 游戏区域宽度
#define HEIGHT 25   // 游戏区域高度
#define MAX_SNAKE 1000

typedef struct {
    int x, y;
} Point;

// 全局变量
Point snake[MAX_SNAKE];
int snake_len;
Point food;
Point portal1, portal2;  // 传送门位置
int portal_life;           // 传送门是否存在（0=无 1=有，不会自动消失）
int portal_timer;          // 距离下次传送门刷新的 tick 数
int portal_blocked;        // 传送门刷新被阻止（蛇离食物太近）
int direction;             // 0=上 1=右 2=下 3=左
int score;
int game_over;
int paused;               // 暂停状态
int speed;                // 游戏速度(毫秒)
int difficulty;           // 0=简单 1=中等 2=困难
int base_speed;           // 初始速度
int speed_step;           // 每吃一个食物减少的毫秒数
int invincible;           // 无敌状态
int invincible_ticks;      // 无敌剩余 tick 数
int invincible_left;       // 无敌剩余秒数（显示用）
int next_milestone;        // 下一个触发无敌的分数
int boost_points;          // 加速点（上限3）
int boost_active;          // 加速是否激活
int boost_ticks;           // 加速剩余 tick
int boost_saved_speed;     // 加速前的原始速度
Point boost_fruit;         // 加速果实位置
int boost_fruit_active;    // 加速果实是否存在
int boost_fruit_timer;     // 加速果实刷新计时器

// 隐藏光标
void hide_cursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(hOut, &info);
    info.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &info);
}

// 设置光标位置
void gotoxy(int x, int y) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, pos);
}

// 将控制台窗口居中
void center_window() {
    HWND hwnd = GetConsoleWindow();
    if (hwnd == NULL) return;
    RECT rect;
    GetWindowRect(hwnd, &rect);
    int win_width = rect.right - rect.left;
    int win_height = rect.bottom - rect.top;
    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    int x = (screen_width - win_width) / 2;
    int y = (screen_height - win_height) / 2;
    SetWindowPos(hwnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
}

// 设置控制台窗口大小
void set_window_size(int cols, int rows) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    // 设置缓冲区大小
    COORD buffer = { (SHORT)cols, (SHORT)(rows + 5) };
    SetConsoleScreenBufferSize(hOut, buffer);
    // 设置窗口大小
    SMALL_RECT window = { 0, 0, (SHORT)(cols - 1), (SHORT)(rows - 1) };
    SetConsoleWindowInfo(hOut, TRUE, &window);
}

// 设置控制台颜色
void set_color(int fg, int bg) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, (WORD)((bg << 4) | fg));
}

// 初始化游戏
void init_game(int diff) {
    difficulty = diff;

    // 根据难度设置速度参数
    switch (difficulty) {
        case 0: // 简单
            base_speed = 250;
            speed_step = 3;
            break;
        case 1: // 中等
            base_speed = 170;
            speed_step = 5;
            break;
        case 2: // 困难
            base_speed = 110;
            speed_step = 8;
            break;
        default:
            base_speed = 170;
            speed_step = 5;
    }

    snake_len = 3;
    // 蛇初始位置（水平，头朝右）
    for (int i = 0; i < snake_len; i++) {
        snake[i].x = WIDTH / 2 - i;
        snake[i].y = HEIGHT / 2;
    }
    direction = 1;  // 初始向右
    score = 0;
    game_over = 0;
    paused = 0;
    speed = base_speed;
    portal_life = 0;
    portal_timer = 0;
    portal_blocked = 0;
    invincible = 0;
    invincible_left = 0;
    next_milestone = 50;
    boost_points = 2;
    boost_active = 0;
    boost_ticks = 0;
    boost_fruit_active = 0;
    boost_fruit_timer = 60 + rand() % 60;  // 约6~12秒后首次出现

    // 生成第一个食物
    srand((unsigned)time(NULL));
    food.x = rand() % (WIDTH - 2) + 1;
    food.y = rand() % (HEIGHT - 2) + 1;
}

// 绘制游戏画面
void draw() {
    gotoxy(0, 0);

    // 绘制顶部边界
    set_color(11, 0);  // 青色
    printf("┌");
    for (int i = 0; i < WIDTH; i++) printf("─");
    printf("┐\n");

    // 绘制中间区域
    for (int y = 0; y < HEIGHT; y++) {
        printf("│");
        for (int x = 0; x < WIDTH; x++) {
            int is_snake = 0;
            int is_head = 0;
            // 检查是否是蛇身
            for (int i = 0; i < snake_len; i++) {
                if (snake[i].x == x && snake[i].y == y) {
                    is_snake = 1;
                    if (i == 0) is_head = 1;
                    break;
                }
            }
            // 检查是否是食物
            int is_food = (food.x == x && food.y == y);
            // 检查是否是传送门
            int is_portal = (portal_life > 0) &&
                ((portal1.x == x && portal1.y == y) ||
                 (portal2.x == x && portal2.y == y));
            // 检查是否是加速果实
            int is_boost = boost_fruit_active &&
                (boost_fruit.x == x && boost_fruit.y == y);

            if (is_head) {
                if (invincible) {
                    // 无敌时蛇头闪烁金色
                    set_color(14, 0);
                } else {
                    set_color(10, 0);  // 绿色蛇头
                }
                // 根据方向显示不同的蛇头
                char head_char;
                switch (direction) {
                    case 0: head_char = '^'; break;
                    case 1: head_char = '>'; break;
                    case 2: head_char = 'v'; break;
                    case 3: head_char = '<'; break;
                    default: head_char = 'O';
                }
                printf("%c", head_char);
            } else if (is_snake) {
                if (invincible) {
                    set_color(6, 0);   // 无敌时蛇身金色
                } else {
                    set_color(2, 0);   // 深绿色蛇身
                }
                printf("o");
            } else if (is_portal) {
                set_color(13, 0);  // 紫色传送门
                printf("◎");
            } else if (is_boost) {
                set_color(11, 0);  // 青色加速果实
                printf("⚡");
            } else if (is_food) {
                set_color(12, 0);  // 红色食物
                printf("♥");
            } else {
                set_color(8, 0);   // 灰色背景点
                printf("·");
            }
        }
        printf("│\n");
    }

    // 绘制底部边界
    printf("└");
    for (int i = 0; i < WIDTH; i++) printf("─");
    printf("┘\n");

    // 显示分数、难度和速度
    set_color(14, 0);  // 黄色
    printf("\n  🏆 分数: %d  |  ", score);

    // 显示难度
    switch (difficulty) {
        case 0: set_color(10, 0); printf("难度: 简单"); break;
        case 1: set_color(14, 0); printf("难度: 中等"); break;
        case 2: set_color(12, 0); printf("难度: 困难"); break;
    }

    set_color(14, 0);
    // 速度等级：1~10，越大越快
    int speed_level = 1 + (base_speed - speed) * 9 / (base_speed - 40);
    if (speed_level < 1) speed_level = 1;
    if (speed_level > 10) speed_level = 10;
    printf("  |  速度: Lv.%d", speed_level);
    // 速度条
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
    set_color(8, 0);
    printf("]");
    set_color(14, 0);
    printf("  |  方向键/WASD 移动  |  P 暂停  |  Q 退出\n");

    // 无敌状态提示
    if (invincible) {
        set_color(0, 14);  // 黄底黑字
        printf("  ⚡ 🛡 无 敌 模 式 !  剩余 %d 秒  🛡 ⚡    \n", invincible_left);
    } else {
        printf("                                               \n");
    }
    set_color(7, 0);

    // 传送门状态提示
    if (portal_life > 0) {
        set_color(13, 0);
        printf("  🔮 传送门已开启!                      \n");
    } else {
        printf("                                               \n");
    }

    // 加速系统状态
    set_color(11, 0);
    printf("  ⚡ 加速点: ");
    for (int i = 0; i < 3; i++) {
        if (i < boost_points)
            printf("●");
        else
            printf("○");
    }
    if (boost_active) {
        int boost_sec = (boost_ticks * speed) / 1000 + 1;
        set_color(14, 0);
        printf("  [加速中! %ds]", boost_sec);
    } else {
        printf("              ");
    }
    set_color(7, 0);
    printf("  [E] 使用加速\n");

    // 暂停提示覆盖层
    if (paused) {
        int center_x = (WIDTH + 2 - 10) / 2;
        int center_y = HEIGHT / 2;
        gotoxy(center_x, center_y - 1);
        set_color(0, 14);  // 黄底黑字
        printf("                    ");
        gotoxy(center_x, center_y);
        printf("  ⏸  游 戏 暂 停  ");
        gotoxy(center_x, center_y + 1);
        printf("  按 P 键继续...   ");
        gotoxy(center_x, center_y + 2);
        printf("                    ");
        set_color(7, 0);
    }
}

// 检查位置是否可放置（不在蛇身、不在食物上、在边界内）
int is_valid_spot(int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return 0;
    if (x == food.x && y == food.y) return 0;
    for (int i = 0; i < snake_len; i++) {
        if (snake[i].x == x && snake[i].y == y) return 0;
    }
    return 1;
}

// 生成新的食物
// 分数 < 50：生成在蛇头附近（集中），便于新手熟悉操作
// 分数 ≥ 50：生成在远离蛇头的位置（分散），增加挑战
void spawn_food() {
    int hx = snake[0].x, hy = snake[0].y;
    int max_attempts = 500;

    if (score < 50) {
        // 集中：在蛇头附近（曼哈顿距离 3~12），逐圈搜索
        for (int r = 3; r <= 12; r++) {
            for (int dx = -r; dx <= r; dx++) {
                for (int dy = -r; dy <= r; dy++) {
                    if (abs(dx) + abs(dy) != r) continue;
                    int cx = hx + dx, cy = hy + dy;
                    if (cx < 0 || cx >= WIDTH || cy < 0 || cy >= HEIGHT) continue;
                    if (!is_valid_spot(cx, cy)) continue;
                    // 不能和加速果实重合
                    if (boost_fruit_active && cx == boost_fruit.x && cy == boost_fruit.y) continue;
                    food.x = cx; food.y = cy;
                    return;
                }
            }
        }
    } else {
        // 分散：尽量远离蛇头（曼哈顿距离 ≥ 10），随机尝试
        for (int a = 0; a < max_attempts; a++) {
            int cx = rand() % (WIDTH - 2) + 1;
            int cy = rand() % (HEIGHT - 2) + 1;
            int dist = abs(cx - hx) + abs(cy - hy);
            if (dist < 10) continue;
            if (!is_valid_spot(cx, cy)) continue;
            if (boost_fruit_active && cx == boost_fruit.x && cy == boost_fruit.y) continue;
            food.x = cx; food.y = cy;
            return;
        }
    }

    // 兜底：全图随机找个能放的位置
    for (int a = 0; a < max_attempts; a++) {
        int cx = rand() % (WIDTH - 2) + 1;
        int cy = rand() % (HEIGHT - 2) + 1;
        if (!is_valid_spot(cx, cy)) continue;
        if (boost_fruit_active && cx == boost_fruit.x && cy == boost_fruit.y) continue;
        food.x = cx; food.y = cy;
        return;
    }

    // 极端情况（蛇占满地图）：强行放一个位置
    food.x = rand() % (WIDTH - 2) + 1;
    food.y = rand() % (HEIGHT - 2) + 1;
}

// 判断从 (sx,sy) 沿方向 dir 能否在 ≤1 次转向内到达 (tx,ty)
int can_reach(int sx, int sy, int dir, int tx, int ty) {
    switch (dir) {
        case 0: return ty <= sy;  // 向上：目标在上方半平面
        case 1: return tx >= sx;  // 向右：目标在右方半平面
        case 2: return ty >= sy;  // 向下：目标在下方半平面
        case 3: return tx <= sx;  // 向左：目标在左方半平面
    }
    return 0;
}

// 生成传送门，返回 1=成功生成，0=不满足有利条件
// 优先级：门1在蛇≤1次转向可达区域 > 两门间距≥8 > 门2靠近食物
// 预判定：传送门必须对玩家有利（蛇能进门，出门后能≤1次转向吃到食物）
int spawn_portals() {
    if (score < 50) {
        portal_life = 0;
        return 0;
    }

    int hx = snake[0].x, hy = snake[0].y;

    // ── 传送门1：必须在蛇≤1次转向可达的半平面内（最高优先级）──
    int x_min, x_max, y_min, y_max;
    switch (direction) {
        case 0: x_min = 0; x_max = WIDTH - 1; y_min = 0; y_max = hy; break;
        case 1: x_min = hx; x_max = WIDTH - 1; y_min = 0; y_max = HEIGHT - 1; break;
        case 2: x_min = 0; x_max = WIDTH - 1; y_min = hy; y_max = HEIGHT - 1; break;
        case 3: x_min = 0; x_max = hx; y_min = 0; y_max = HEIGHT - 1; break;
        default: x_min = 0; x_max = WIDTH - 1; y_min = 0; y_max = HEIGHT - 1;
    }

    // 在可达区域内随机搜索有效位置
    int found1 = 0;
    for (int attempt = 0; attempt < 500; attempt++) {
        int cx = x_min + rand() % (x_max - x_min + 1);
        int cy = y_min + rand() % (y_max - y_min + 1);
        if (is_valid_spot(cx, cy)) {
            portal1.x = cx; portal1.y = cy;
            found1 = 1; break;
        }
    }
    if (!found1) {
        do {
            portal1.x = rand() % (WIDTH - 2) + 1;
            portal1.y = rand() % (HEIGHT - 2) + 1;
        } while (!is_valid_spot(portal1.x, portal1.y));
    }

    // ── 传送门2：优先保证与门1间距≥8，其次尽量靠近食物 ──
    int found2 = 0;
    for (int r = 2; r <= 8 && !found2; r++) {
        for (int dx = -r; dx <= r && !found2; dx++) {
            for (int dy = -r; dy <= r && !found2; dy++) {
                if (abs(dx) + abs(dy) != r) continue;
                int cx = food.x + dx, cy = food.y + dy;
                if (!is_valid_spot(cx, cy)) continue;
                if (abs(cx - portal1.x) + abs(cy - portal1.y) < 8) continue;
                portal2.x = cx; portal2.y = cy;
                found2 = 1;
            }
        }
    }
    if (!found2) {
        do {
            portal2.x = rand() % (WIDTH - 2) + 1;
            portal2.y = rand() % (HEIGHT - 2) + 1;
        } while (!is_valid_spot(portal2.x, portal2.y) ||
                 abs(portal2.x - portal1.x) + abs(portal2.y - portal1.y) < 8);
    }

    // ── 预判定③：传送门必须对玩家有利 ──
    // 条件A：蛇能从当前位置≤1次转向进入传送门1
    if (!can_reach(hx, hy, direction, portal1.x, portal1.y)) {
        portal_life = 0;
        return 0;
    }
    // 条件B：从传送门2出来后，≤1次转向能到达食物
    if (!can_reach(portal2.x, portal2.y, direction, food.x, food.y)) {
        portal_life = 0;
        return 0;
    }

    portal_life = 1;
    portal_blocked = 0;
    return 1;
}

// 生成加速果实：位置与传送门1相反（蛇的后方半平面），让玩家难以吃到
void spawn_boost_fruit() {
    if (boost_fruit_active || boost_points >= 3) return;

    int hx = snake[0].x, hy = snake[0].y;
    int x_min, x_max, y_min, y_max;
    // 与 can_reach 相反：取蛇后方的半平面
    switch (direction) {
        case 0: x_min = 0; x_max = WIDTH - 1; y_min = hy + 1; y_max = HEIGHT - 1; break;  // 上→后方是下
        case 1: x_min = 0; x_max = hx - 1;    y_min = 0; y_max = HEIGHT - 1; break;       // 右→后方是左
        case 2: x_min = 0; x_max = WIDTH - 1; y_min = 0; y_max = hy - 1; break;            // 下→后方是上
        case 3: x_min = hx + 1; x_max = WIDTH - 1; y_min = 0; y_max = HEIGHT - 1; break;   // 左→后方是右
        default: x_min = 0; x_max = WIDTH - 1; y_min = 0; y_max = HEIGHT - 1;
    }

    // 检查后方半平面是否有效（蛇在边界时后方可能无空间）
    if (x_min > x_max || y_min > y_max) {
        // 后方无空间，全图随机生成
        x_min = 0; x_max = WIDTH - 1;
        y_min = 0; y_max = HEIGHT - 1;
    }

    int valid = 0;
    for (int attempt = 0; attempt < 300; attempt++) {
        int cx = x_min + rand() % (x_max - x_min + 1);
        int cy = y_min + rand() % (y_max - y_min + 1);
        if (cx < 0 || cx >= WIDTH || cy < 0 || cy >= HEIGHT) continue;
        // 不在蛇身上
        int on_snake = 0;
        for (int i = 0; i < snake_len; i++) {
            if (snake[i].x == cx && snake[i].y == cy) { on_snake = 1; break; }
        }
        if (on_snake) continue;
        // 不在食物上
        if (cx == food.x && cy == food.y) continue;
        // 不在传送门上
        if (portal_life && ((cx == portal1.x && cy == portal1.y) ||
                            (cx == portal2.x && cy == portal2.y))) continue;
        boost_fruit.x = cx;
        boost_fruit.y = cy;
        valid = 1;
        break;
    }
    if (valid) {
        boost_fruit_active = 1;
    }
}

// 更新游戏逻辑
void update() {
    // 更新无敌倒计时（基于游戏tick，暂停时自然停止）
    if (invincible) {
        invincible_ticks--;
        invincible_left = (invincible_ticks * speed) / 1000 + 1;
        if (invincible_ticks <= 0) {
            invincible = 0;
            invincible_left = 0;
        }
    }

    // 传送门生命周期管理（分数>=50时启用）
    if (score >= 50) {
        if (portal_life > 0) {
            // 传送门持续存在，不会自动消失
        } else if (portal_timer > 0) {
            portal_timer--;
            if (portal_timer <= 0) {
                int dist = abs(snake[0].x - food.x) + abs(snake[0].y - food.y);
                // 条件①：蛇离食物太近 → 不生成
                if (dist < 5) {
                    portal_blocked = 1;
                }
                // 条件②：蛇已经能≤1次转向吃到食物 → 不需要传送门
                else if (can_reach(snake[0].x, snake[0].y, direction, food.x, food.y)) {
                    portal_blocked = 1;
                }
                // 条件③：尝试生成，内部预判定是否有利
                else {
                    if (!spawn_portals()) {
                        // 生成失败（不满足有利条件），阻止并等待下次机会
                        portal_blocked = 1;
                    }
                }
            }
        } else {
            // 首次达到50分，开始计时
            portal_timer = 20 + rand() % 50;
        }
    }

    // 加速果实刷新计时
    if (!boost_fruit_active && boost_points < 3) {
        boost_fruit_timer--;
        if (boost_fruit_timer <= 0) {
            spawn_boost_fruit();
            boost_fruit_timer = 80 + rand() % 120;
        }
    }

    // 加速状态倒计时
    if (boost_active) {
        boost_ticks--;
        if (boost_ticks <= 0) {
            boost_active = 0;
            speed = boost_saved_speed;
        }
    }

    // 计算新蛇头位置
    Point new_head = snake[0];
    switch (direction) {
        case 0: new_head.y--; break;  // 上
        case 1: new_head.x++; break;  // 右
        case 2: new_head.y++; break;  // 下
        case 3: new_head.x--; break;  // 左
    }

    // 检查是否撞墙
    if (new_head.x < 0 || new_head.x >= WIDTH ||
        new_head.y < 0 || new_head.y >= HEIGHT) {
        if (invincible) {
            // 无敌时穿墙：从对面出现
            if (new_head.x < 0) new_head.x = WIDTH - 1;
            if (new_head.x >= WIDTH) new_head.x = 0;
            if (new_head.y < 0) new_head.y = HEIGHT - 1;
            if (new_head.y >= HEIGHT) new_head.y = 0;
        } else {
            game_over = 1;
            return;
        }
    }

    // 检查是否撞到自己
    for (int i = 0; i < snake_len - 1; i++) {
        if (snake[i].x == new_head.x && snake[i].y == new_head.y) {
            if (!invincible) {
                game_over = 1;
                return;
            }
        }
    }

    // 移动蛇：所有身体前移
    for (int i = snake_len; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    snake[0] = new_head;

    // 检查是否进入传送门
    if (portal_life > 0) {
        if (new_head.x == portal1.x && new_head.y == portal1.y) {
            // 从传送门1传送到传送门2（靠近食物的那个）
            snake[0].x = portal2.x;
            snake[0].y = portal2.y;
            portal_life = 0;
            portal_timer = 30 + rand() % 80;  // 使用后随机间隔再刷新
        } else if (new_head.x == portal2.x && new_head.y == portal2.y) {
            // 从传送门2传送到传送门1
            snake[0].x = portal1.x;
            snake[0].y = portal1.y;
            portal_life = 0;
            portal_timer = 30 + rand() % 80;
        }
    }

    // 检查是否吃到加速果实
    if (boost_fruit_active &&
        snake[0].x == boost_fruit.x && snake[0].y == boost_fruit.y) {
        if (boost_points < 3) {
            boost_points++;
        }
        boost_fruit_active = 0;
        boost_fruit_timer = 80 + rand() % 120;
    }

    // 检查是否吃到食物
    if (snake[0].x == food.x && snake[0].y == food.y) {
        snake_len++;
        score += 10;

        // 传送门处理：食物被吃后传送门消失
        if (portal_life > 0) {
            portal_life = 0;
            portal_timer = 30 + rand() % 80;
        }
        // 传送门被阻止 → 吃掉食物后立即尝试生成
        if (portal_blocked) {
            if (!spawn_portals()) {
                // 预判定失败，开始正常计时等待下次机会
                portal_timer = 20 + rand() % 50;
            }
        }

        spawn_food();

        // 检查是否触发无敌（每50分）
        if (score >= next_milestone) {
            invincible = 1;
            invincible_ticks = 10000 / speed;  // 10秒对应的tick数
            invincible_left = 10;
            next_milestone += 50;
        }

        // 每吃一个食物就逐渐加速（最低 40ms）
        if (speed > 40) {
            speed -= speed_step;
            if (speed < 40) speed = 40;
        }
    }
}

// 处理输入
void input() {
    if (_kbhit()) {
        int key = _getch();
        if (key == 224) {  // 方向键（双字节）
            key = _getch();
            if (paused) return;  // 暂停时忽略方向键
            switch (key) {
                case 72: if (direction != 2) direction = 0; break;  // 上
                case 80: if (direction != 0) direction = 2; break;  // 下
                case 75: if (direction != 1) direction = 3; break;  // 左
                case 77: if (direction != 3) direction = 1; break;  // 右
            }
        } else {
            // P 暂停/继续
            if (key == 'p' || key == 'P') {
                paused = !paused;
                return;
            }
            // E 加速技能
            if (key == 'e' || key == 'E') {
                if (!boost_active && boost_points > 0 && !paused) {
                    boost_active = 1;
                    boost_points--;
                    boost_saved_speed = speed;
                    speed = speed * 2 / 3;  // 速度加快50%（间隔缩短）
                    boost_ticks = 5000 / speed;  // 5秒对应的tick
                }
                return;
            }
            // 暂停时忽略其他按键（除了 Q）
            if (paused) {
                if (key == 'q' || key == 'Q') game_over = 1;
                return;
            }
            // WASD 和 Q
            switch (key) {
                case 'w': case 'W': if (direction != 2) direction = 0; break;
                case 's': case 'S': if (direction != 0) direction = 2; break;
                case 'a': case 'A': if (direction != 1) direction = 3; break;
                case 'd': case 'D': if (direction != 3) direction = 1; break;
                case 'q': case 'Q': game_over = 1; break;
            }
        }
    }
}

// 游戏结束画面
void show_game_over() {
    system("cls");
    set_color(12, 0);
    printf("\n\n");
    printf("  ╔═══════════════════════════════╗\n");
    printf("  ║                               ║\n");
    printf("  ║       游  戏  结  束  !       ║\n");
    printf("  ║                               ║\n");
    printf("  ╚═══════════════════════════════╝\n");

    // 根据难度显示不同颜色
    set_color(14, 0);
    printf("\n         🏆 最终得分: %d\n", score);
    switch (difficulty) {
        case 0: set_color(10, 0); printf("         📗 难度: 简单\n"); break;
        case 1: set_color(14, 0); printf("         📙 难度: 中等\n"); break;
        case 2: set_color(12, 0); printf("         📕 难度: 困难\n"); break;
    }
    set_color(7, 0);
    printf("\n    按 R 重新开始  |  按 Q 返回主页\n");
}

// 玩法介绍页面
void show_help() {
    system("cls");
    set_color(11, 0);
    printf("\n");
    printf("  ╔══════════════════════════════════════════════╗\n");
    printf("  ║                                              ║\n");
    printf("  ║           📖  游  戏  玩  法  📖             ║\n");
    printf("  ║                                              ║\n");
    printf("  ╚══════════════════════════════════════════════╝\n\n");

    set_color(14, 0);
    printf("  🎯 【游戏目标】\n");
    set_color(7, 0);
    printf("    控制蛇吃掉红色的 ♥ 食物，每吃一个得 10 分。\n");
    printf("    蛇会越来越长，速度越来越快！\n\n");

    set_color(14, 0);
    printf("  🎮 【操作方法】\n");
    set_color(7, 0);
    printf("    ↑↓←→ 或 W A S D  ——  控制蛇的移动方向\n");
    printf("    P                  ——  暂停 / 继续游戏\n");
    printf("    Q                  ——  退出游戏\n\n");

    set_color(14, 0);
    printf("  ⚡ 【游戏机制】\n");
    set_color(7, 0);
    printf("    💀 撞墙或撞到自己身体 → 游戏结束\n");
    printf("    🛡 每 50 分获得 10 秒无敌：穿墙、不自杀\n");
    printf("    🔮 分数 ≥ 50 后随机出现传送门（紫色 ◎）\n");
    set_color(8, 0);
    printf("       传送门成对出现，一个在蛇前方、一个靠近食物\n");
    printf("       进入传送门会瞬间传送到另一个门的位置\n");
    set_color(7, 0);
    printf("    ⚠ 蛇离食物太近时传送门不会出现\n\n");

    set_color(14, 0);
    printf("  📊 【难度选择】\n");
    set_color(10, 0);
    printf("    🟢 简单：初始 Lv.1，加速慢，适合新手\n");
    set_color(14, 0);
    printf("    🟡 中等：初始 Lv.3，加速适中，有挑战性\n");
    set_color(12, 0);
    printf("    🔴 困难：初始 Lv.5，加速快，高手专属\n\n");

    set_color(14, 0);
    printf("  💡 【通关技巧】\n");
    set_color(7, 0);
    printf("    · 利用传送门快速接近远处的食物\n");
    printf("    · 无敌时大胆穿墙抄近路\n");
    printf("    · 绕大圈给自己留出反应空间\n");
    printf("    · 不要在蛇身密集区域急转弯\n\n");

    set_color(11, 0);
    printf("  ═══════════════════════════════════════════════\n");
    set_color(7, 0);
    printf("           按任意键返回主菜单...");
    _getch();
}

// 显示欢迎页面并选择难度，返回难度（0/1/2），Q 则直接退出程序
int welcome_screen() {
    system("cls");
    center_window();

    printf("\n\n");
    printf("  ╔═══════════════════════════════╗\n");
    printf("  ║                               ║\n");
    printf("  ║        🐍  贪  吃  蛇  🐍     ║\n");
    printf("  ║                               ║\n");
    printf("  ╚═══════════════════════════════╝\n");

    printf("\n    请选择难度:\n\n");
    printf("    [1] 🟢 简单  - 初始 Lv.1（较慢），每次加速 +1\n");
    printf("    [2] 🟡 中等  - 初始 Lv.3（适中），每次加速 +1\n");
    printf("    [3] 🔴 困难  - 初始 Lv.5（较快），每次加速 +2\n\n");
    printf("    操作: 方向键 / WASD 移动，P 暂停，Q 退出\n");
    printf("          H 查看玩法介绍\n\n");
    printf("    请输入 1/2/3 选择难度（或 H 查看玩法）: ");

    while (1) {
        int key = _getch();
        if (key == '1') return 0;
        if (key == '2') return 1;
        if (key == '3') return 2;
        if (key == 'q' || key == 'Q') {
            system("cls");
            set_color(7, 0);
            printf("\n  感谢游玩！再见！👋\n\n");
            exit(0);
        }
        if (key == 'h' || key == 'H') {
            show_help();
            // 返回后重新显示菜单
            system("cls");
            center_window();
            printf("\n\n");
            printf("  ╔═══════════════════════════════╗\n");
            printf("  ║                               ║\n");
            printf("  ║        🐍  贪  吃  蛇  🐍     ║\n");
            printf("  ║                               ║\n");
            printf("  ╚═══════════════════════════════╝\n");
            printf("\n    请选择难度:\n\n");
            printf("    [1] 🟢 简单  - 初始 Lv.1（较慢），每次加速 +1\n");
            printf("    [2] 🟡 中等  - 初始 Lv.3（适中），每次加速 +1\n");
            printf("    [3] 🔴 困难  - 初始 Lv.5（较快），每次加速 +2\n\n");
            printf("    操作: 方向键 / WASD 移动，P 暂停，Q 退出\n");
            printf("          H 查看玩法介绍\n\n");
            printf("    请输入 1/2/3 选择难度（或 H 查看玩法）: ");
        }
    }
}

int main() {
    system("chcp 65001 >nul");  // 设置 UTF-8 编码

    while (1) {
        int diff = welcome_screen();

        int restart;
        do {
            restart = 0;
            hide_cursor();
            system("cls");
            init_game(diff);

            // 游戏主循环
            while (!game_over) {
                draw();
                input();
                if (!paused) {
                    update();
                }
                // 在等待期间也检测输入
                for (int i = 0; i < speed / 10; i++) {
                    input();
                    if (game_over) break;
                    if (paused) { i--; continue; }
                    Sleep(10);
                }
            }

            show_game_over();

            // 等待用户选择
            while (1) {
                if (_kbhit()) {
                    int key = _getch();
                    if (key == 'r' || key == 'R') {
                        restart = 1;
                        break;
                    }
                    if (key == 'q' || key == 'Q') {
                        restart = 0;
                        break;
                    }
                }
            }
            system("cls");
        } while (restart);
        // Q 在游戏结束界面 → 回到欢迎页
    }
    return 0;
}