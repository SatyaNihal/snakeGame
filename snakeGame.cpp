#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <string>
#include <vector>
using namespace std;

const int gameWidth = 40;
const int gameHeight = 20;

int headX, headY;
int foodX, foodY;
int score;
int tailX[100], tailY[100];
int tailLength;
enum eDirection
{
    NONE = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
eDirection direction;
bool gameOver;

const string RESET = "\033[0m";
const string GREEN = "\033[32m";
const string RED = "\033[31m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string CYAN = "\033[36m";
const string BOLD = "\033[1m";

void initGame()
{
    gameOver = false;
    direction = RIGHT;
    headX = gameWidth / 2;
    headY = gameHeight / 2;
    foodX = rand() % gameWidth;
    foodY = rand() % gameHeight;
    score = 0;
    tailLength = 0;
}

int keyHit()
{
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

char getChar()
{
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}

void renderGame(const string &user)
{
    system("clear");

    cout << BOLD << BLUE << "\n╔════════════════════════════════════════════╗" << RESET << endl;
    cout << BOLD << BLUE << "║             " << CYAN << "S N A K E  G A M E" << BLUE << "             ║" << RESET << endl;
    cout << BOLD << BLUE << "╚════════════════════════════════════════════╝" << RESET << endl
         << endl;

    cout << CYAN << "╔";
    for (int i = 0; i < gameWidth; i++)
        cout << "═";
    cout << "╗" << RESET << endl;

    for (int i = 0; i < gameHeight; i++)
    {
        cout << CYAN << "║" << RESET;
        for (int j = 0; j < gameWidth; j++)
        {
            if (i == headY && j == headX)
                cout << GREEN << "O" << RESET;
            else if (i == foodY && j == foodX)
                cout << RED << "♦" << RESET;
            else
            {
                bool printed = false;
                for (int k = 0; k < tailLength; k++)
                {
                    if (tailX[k] == j && tailY[k] == i)
                    {
                        cout << GREEN << "o" << RESET;
                        printed = true;
                        break;
                    }
                }
                if (!printed)
                    cout << " ";
            }
        }
        cout << CYAN << "║" << RESET << endl;
    }

    cout << CYAN << "╚";
    for (int i = 0; i < gameWidth; i++)
        cout << "═";
    cout << "╝" << RESET << endl;

    cout << YELLOW << BOLD << "\n"
         << user << "'s Score: " << score << RESET << endl;
    cout << "\nControls: " << BOLD << "W" << RESET << " (Up), "
         << BOLD << "S" << RESET << " (Down), "
         << BOLD << "A" << RESET << " (Left), "
         << BOLD << "D" << RESET << " (Right), "
         << BOLD << "X" << RESET << " (Quit)" << endl;
}

void updateGame()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = headX;
    tailY[0] = headY;

    for (int i = 1; i < tailLength; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (direction)
    {
    case LEFT:
        headX--;
        break;
    case RIGHT:
        headX++;
        break;
    case UP:
        headY--;
        break;
    case DOWN:
        headY++;
        break;
    default:
        break;
    }

    if (headX >= gameWidth || headX < 0 || headY >= gameHeight || headY < 0)
    {
        gameOver = true;
    }

    for (int i = 0; i < tailLength; i++)
    {
        if (tailX[i] == headX && tailY[i] == headY)
            gameOver = true;
    }

    if (headX == foodX && headY == foodY)
    {
        score += 10;
        bool validPosition;
        do
        {
            validPosition = true;
            foodX = rand() % gameWidth;
            foodY = rand() % gameHeight;

            if (foodX == headX && foodY == headY)
            {
                validPosition = false;
                continue;
            }

            for (int i = 0; i < tailLength; i++)
            {
                if (foodX == tailX[i] && foodY == tailY[i])
                {
                    validPosition = false;
                    break;
                }
            }
        } while (!validPosition);

        tailLength++;
    }
}

int setDifficulty()
{
    int level;
    cout << CYAN << BOLD << "\n╔════════════════════════════════════╗" << RESET << endl;
    cout << CYAN << BOLD << "║        " << YELLOW << "DIFFICULTY SETTINGS" << CYAN << "         ║" << RESET << endl;
    cout << CYAN << BOLD << "╚════════════════════════════════════╝" << RESET << endl
         << endl;

    cout << "1: " << GREEN << "Easy" << RESET << " - Slower snake speed" << endl;
    cout << "2: " << YELLOW << "Medium" << RESET << " - Standard snake speed" << endl;
    cout << "3: " << RED << "Hard" << RESET << " - Fast snake speed" << endl;
    cout << "\nChoose difficulty level (1-3): ";

    cin >> level;
    if (level < 1 || level > 3)
    {
        cout << "Invalid choice. Setting to Medium difficulty." << endl;
        level = 2;
        sleep(1);
    }

    switch (level)
    {
    case 1:
        return 150;
    case 3:
        return 50;
    default:
        return 100;
    }
}

void showInstructions()
{
    system("clear");
    cout << BLUE << BOLD << "\n╔════════════════════════════════════════════╗" << RESET << endl;
    cout << BLUE << BOLD << "║            " << CYAN << "GAME INSTRUCTIONS" << BLUE << "               ║" << RESET << endl;
    cout << BLUE << BOLD << "╚════════════════════════════════════════════╝" << RESET << endl
         << endl;

    cout << "• Control the snake using " << BOLD << "W" << RESET << " (Up), "
         << BOLD << "S" << RESET << " (Down), "
         << BOLD << "A" << RESET << " (Left), "
         << BOLD << "D" << RESET << " (Right)" << endl;
    cout << "• Press " << BOLD << "X" << RESET << " at any time to quit the game" << endl;
    cout << "• Eat food (" << RED << "♦" << RESET << ") to grow longer and earn points" << endl;
    cout << "• Avoid colliding with the tail or going outside the box" << endl;

    cout << "\nPress Enter to continue...";
    cin.ignore(100, '\n');
    cin.get();
}

void processInput()
{
    if (keyHit())
    {
        char key = getChar();
        switch (key)
        {
        case 'a':
            if (direction != RIGHT)
                direction = LEFT;
            break;
        case 'd':
            if (direction != LEFT)
                direction = RIGHT;
            break;
        case 'w':
            if (direction != DOWN)
                direction = UP;
            break;
        case 's':
            if (direction != UP)
                direction = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}

void showGameOver(const string &userName, int finalScore)
{
    system("clear");
    cout << RED << BOLD << "\n╔════════════════════════════════════════════╗" << RESET << endl;
    cout << RED << BOLD << "║               " << "GAME OVER!" << "                   ║" << RESET << endl;
    cout << RED << BOLD << "╚════════════════════════════════════════════╝" << RESET << endl
         << endl;

    cout << YELLOW << "Player: " << BOLD << userName << RESET << endl;
    cout << YELLOW << "Final Score: " << BOLD << finalScore << RESET << endl;

    if (finalScore < 50)
        cout << "\nKeep practicing!" << endl;
    else if (finalScore < 100)
        cout << "\nNice job!" << endl;
    else
        cout << "\nAmazing performance!" << endl;

    cout << "\nPress Enter to exit...";
    cin.ignore(100, '\n');
    cin.get();
}

int main()
{
    srand(time(0));
    string userName;

    cout << CYAN << BOLD << "\n╔════════════════════════════════════════════╗" << RESET << endl;
    cout << CYAN << BOLD << "║            " << "WELCOME TO SNAKE" << "                ║" << RESET << endl;
    cout << CYAN << BOLD << "╚════════════════════════════════════════════╝" << RESET << endl
         << endl;

    cout << "Enter your name: ";
    cin >> userName;

    showInstructions();
    int delay = setDifficulty();

    initGame();
    while (!gameOver)
    {
        renderGame(userName);
        processInput();
        updateGame();
        usleep(delay * 1000);
    }

    showGameOver(userName, score);
    return 0;
}
