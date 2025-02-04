#include <ncurses.h>
#include <queue>
#include <cstdlib> // 生成随机数
#include <ctime>   // 获取当前时间用于随机种子

using namespace std;

// 结构体表示点的坐标
struct Point {
    int x;
    int y;
};

// 初始化时长为1，位置在原点
Point snake[1024]; // 预留足够空间，实际长度可以用一个变量存储
int snake_length = 1;

// 食物的位置和数量
int food_x, food_y;
bool has_food = false;

// 随机种子为当前时间
srand(time(0));

// 设置游戏区域大小（例如20x20）
const int GAME_WIDTH = 20;
const int GAME_HEIGHT = 20;

// 初始化窗口
void init_game() {
    initscr(); // 初始化ncurses

    // 清除屏幕
    erase();
}

// 结束游戏并清理窗口
void end_game() {
    clear();
    getchar();
    delwin(stdin);
    return;
}

// 检查点是否在游戏区域内
bool is_in_bounds(const Point& p) {
    if (p.x < 0 || p.x >= GAME_WIDTH || p.y < 0 || p.y >= GAME_HEIGHT)
        return false;
    return true;
}

// 随机生成食物位置，确保不与蛇冲突
void generate_food() {
    do {
        food_x = rand() % (GAME_WIDTH);
        food_y = rand() % (GAME_HEIGHT);
    } while(find(snake, snake + snake_length - 1, Point(), &Point::x) != -1 &&
             food_x == food[0].x && food_y == food[0].y);

    has_food = true;
}

// 移动蛇，并处理吃食物的情况
void move_snake() {
    Point new_head;
    if (has_food) {
        // 吃掉食物，增加长度
        snake_length++;
        new_head.x = snake[snake_length - 2].x == food_x ? food_x : snake[snake_length - 1].x;
        new_head.y = snake[snake_length - 2].y == food_y ? food_y : snake[snake_length - 1].y;

        // 在蛇头后面插入新头部
        if (new_head.x == snake[0].x && new_head.y == snake[0].y)
            erase();
    } else {
        // 未吃到食物，移动一格
        new_head = {snake[0].x + 1, snake[0].y};
        if (!is_in_bounds(new_head)) {
            end_game();
            return;
        }
    }

    Point old_head = snake[0];
    snake[0] = new_head;

    // 检查是否自相撞
    bool collision = false;
    for (int i = 1; i < snake_length; ++i) {
        if (new_head.x == snake[i].x && new_head.y == snake[i].y) {
            collision = true;
            break;
        }
    }

    if (collision) {
        end_game();
        return;
    }

    // 移除旧的头部（如果没有吃到食物）
    if (!has_food) {
        Point old_tail = snake[snake_length - 1];
        erase();
        write(' ', old_tail.x);
        write(' ', old_tail.y);
        snake[snake_length] = '\0';
    }
}

// 绘制游戏界面
void draw_game() {
    clear();

    // 画出蛇
    for (int i = 0; i < snake_length; ++i) {
        if (i == 0)
            write('S', snake[i].x, snake[i].y);
        else
            write('
```, snake[i].x, snake[i].y);
    }

    // 画出食物
    write('*', food_x, food_y);

    refresh();
}

// 主控制循环
void game_loop() {
    while (true) {
        draw_game();

        int ch = getchar();
        if ((ch == 'Q') || (ch == 'q')) {
            end_game();
            break;
        }

        switch (ch) {
            case 'ArrowUp':
                // 上方向按下
                new_head.y--;
                if (!is_in_bounds(new_head)) {
                    end_game();
                    return;
                }
                // 检查是否与其他部分重叠
                for (int i = 1; i < snake_length; ++i) {
                    if (new_head.x == snake[i].x && new_head.y == snake[i].y) {
                        end_game();
                        return;
                    }
                }
                // 移动
                snake[0] = new_head;
                break;

            case 'ArrowDown':
                // 下方向按下
                new_head.y++;
                if (!is_in_bounds(new_head)) {
                    end_game();
                    return;
                }
                for (int i = 1; i < snake_length; ++i) {
                    if (new_head.x == snake[i].x && new_head.y == snake[i].y) {
                        end_game();
                        return;
                    }
                }
                snake[0] = new_head;
                break;

            case 'ArrowLeft':
                // 左方向按下
                new_head.x--;
                if (!is_in_bounds(new_head)) {
                    end_game();
                    return;
                }
                for (int i = 1; i < snake_length; ++i) {
                    if (new_head.x == snake[i].x && new_head.y == snake[i].y) {
                        end_game();
                        return;
                    }
                }
                snake[0] = new_head;
                break;

            case 'ArrowRight':
                // 右方向按下
                new_head.x++;
                if (!is_in_bounds(new_head)) {
                    end_game();
                    return;
                }
                for (int i = 1; i < snake_length; ++i) {
                    if (new_head.x == snake[i].x && new_head.y == snake[i].y) {
                        end_game();
                        return;
                    }
                }
                snake[0] = new_head;
                break;

            default:
                // 未移动
                break;
        }

        // 如果没有吃到食物，移除尾部
        if (!has_food) {
            Point old_tail = snake[snake_length - 1];
            erase();
            write(' ', old_tail.x);
            write(' ', old_tail.y);
            snake[snake_length] = '\0';
        }

        has_food = false;
    }
}

// 启动游戏
int main() {
    init_game();

    // 初始化初始位置
    Point initial = {10, 10};
    snake[0] = initial;

    // 进入循环
    game_loop();

    return 0;
}
