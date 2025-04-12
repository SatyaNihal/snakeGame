# Snake Game

A classic Snake game implemented in C++ for the terminal. Control a snake to eat food, grow longer, and score points while avoiding collisions with the walls or your own tail. Created by **Satya Nihal Kodukula** to make use of low-level programming knowledge learnt in **CISC 221, Computer Architecture.**

## Features

- **Real-Time Controls**: Move the snake in four directions (W, A, S, D) without pressing Enter each time.
- **Difficulty Settings**: Choose between three difficulty levels : Easy (150 ms delay), Medium (100 ms delay), Hard (50 ms delay). 
- **Score Tracking**: Gain 10 points for each piece of food eaten.
- **Game Over Screen**: See your final score along with a short message based on performance.

## How It Works

1. **Initialization**:
   - The game sets up the snake’s head at the center of the play area.
   - The first food item is placed randomly.
   - Score starts at 0, and the snake’s length starts at 0 (just the head).

2. **Rendering**:
   - The screen is cleared each frame, and a border is drawn.
   - The snake’s head (`O`) and tail (`o`) are displayed, along with the food item (`♦`).

3. **Movement**:
   - Press **W** to move up, **S** to move down, **A** to move left, **D** to move right.
   - The tail follows the head. Each piece of the tail moves to the position previously occupied by the segment ahead of it.

4. **Collision Detection**:
   - Hitting any boundary (left/right edges or top/bottom) results in **Game Over**.
   - Colliding with your own tail also ends the game.

5. **Score Logic**:
   - When the snake’s head overlaps the food’s position, the score is increased by 10 points.
   - A new food position is chosen randomly on the board, ensuring it does not overlap with the snake.

6. **Exiting**:
   - Press **X** at any time to quit the game immediately.

## Requirements

- A **Unix-like operating system** (Linux, macOS, etc.) due to the use of headers like `<unistd.h>`, `<termios.h>`, `<fcntl.h>`.
- A modern **C++ compiler** such as `g++`.

## Compilation

```bash
g++ -o snake snake.cpp
