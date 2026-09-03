# C 语言学习练习 (C_project)

这是我学习 C 语言过程中的练习代码仓库，包含教材章节习题、课堂作业以及几个自己写的控制台小程序（含两个"自制游戏"）。

## 目录结构

```
C_project/
├── unit2/          # 第 2 章习题（q1 ~ q8）
├── unit3/          # 第 3 章习题（q1）
├── game.c          # 2D 贪吃蛇（带传送门）
├── game3d.c        # 3D 贪吃蛇（多层空间）
└── *.c             # 根目录下其余独立练习
```

## 内容一览

| 文件 | 说明 |
|---|---|
| `hello.c` / `tom.c` / `test.c` / `test2.c` | Hello World 入门练习 |
| `q1.c` / `q2.c` / `q3.c` | 字符图案（星号菱形）输出练习 |
| `work1.c` / `work2.c` | 输入三个数输出最大值（练习 if / else if） |
| `change.c` / `change_calculator.c` / `change_calculator2.c` | 找零计算器 |
| `abc.c` / `c.c` / `h.c` / `0or1.c` | 其他零散小练习 |
| `game.c` | **2D 贪吃蛇**：方向键控制、食物、得分、传送门 |
| `game3d.c` | **3D 贪吃蛇**：在 3 层立体空间里吃食物成长 |
| `unit2/q1.c ~ q8.c` | 第 2 章习题：星号图案、贷款余额按月计算等 |
| `unit3/q1.c` | 第 3 章习题：日期格式化输出 (yyyy/mm/dd) |
| `compile.bat` | 用本机 MinGW 编译 `change_calculator.c` 并输出错误信息 |

## 运行环境

- **系统**：Windows（游戏的 `game.c` / `game3d.c` 使用了 `conio.h`、`windows.h`，仅在 Windows 上编译运行）
- **编译器**：MinGW-w64 / GCC，或 Visual Studio
- **语言标准**：C99 及以上

单个文件编译运行示例：

```bash
gcc game.c -o game.exe
game.exe
```

## 关于两个小游戏

- `game.c`（2D 贪吃蛇）：体素风格的控制台贪吃蛇，吃掉食物增长得分，地图上还会出现**传送门**，穿过去可快速移动。
- `game3d.c`（3D 贪吃蛇）：在三维空间中操控贪吃蛇，需要切换"层"来吃到不同深度的食物，难度更高。

## 说明

本仓库为个人学习用途，题目大多来自课堂作业与教材（K.N. King《C Programming: A Modern Approach》风格的课后题），代码仅供学习参考。
