#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <string>
#include <ctime>

using namespace std;

// Убираем мигание экрана
void setCursor(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

struct Point { int x, y; };

class Snake {
public:
    vector<Point> body;
    Snake() {
        body.push_back({ 10, 10 }); // Начальная позиция
    }

    void move(int dx, int dy, bool grow) {
        Point nextHead = { body[0].x + dx, body[0].y + dy };
        body.insert(body.begin(), nextHead); // Добавляем новую голову
        if (!grow) {
            body.pop_back(); // Если не ели — удаляем хвост (движение)
        }
        // Если grow == true, pop_back не вызывается -> змейка удлиняется
    }
};

int main() {
    srand(time(0));
    hideCursor();

    Snake snake;
    Point food = { rand() % 18 + 1, rand() % 18 + 1 };
    int score = 0;
    int dx = 0, dy = 1; // Направление: вправо по умолчанию
    char key = 'd', lastKey = 'd';

    while (true) {
        setCursor(0, 0);

        // 1. Управление (защита от разворота на 180)
        if (_kbhit()) {
            char temp = _getch();
            if (temp == 'w' && lastKey != 's') { key = 'w'; dx = -1; dy = 0; lastKey = 'w'; }
            else if (temp == 's' && lastKey != 'w') { key = 's'; dx = 1; dy = 0; lastKey = 's'; }
            else if (temp == 'a' && lastKey != 'd') { key = 'a'; dx = 0; dy = -1; lastKey = 'a'; }
            else if (temp == 'd' && lastKey != 'a') { key = 'd'; dx = 0; dy = 1; lastKey = 'd'; }
        }

        // 2. Логика поедания еды
        bool grow = false;
        if (snake.body[0].x == food.x && snake.body[0].y == food.y) {
            score++;
            grow = true;
            food = { rand() % 18 + 1, rand() % 18 + 1 };
        }

        // 3. Движение
        snake.move(dx, dy, grow);

        // 4. Проверка смерти (стены и хвост)
        if (snake.body[0].x <= 0 || snake.body[0].x >= 19 || snake.body[0].y <= 0 || snake.body[0].y >= 19) {
            system("cls");
            cout << "GAME OVER! Wall hit. Score: " << score << endl;
            break;
        }
        for (size_t i = 1; i < snake.body.size(); i++) {
            if (snake.body[0].x == snake.body[i].x && snake.body[0].y == snake.body[i].y) {
                system("cls");
                cout << "GAME OVER! Self-eat. Score: " << score << endl;
                return 0;
            }
        }

        // 5. Отрисовка (через строку, чтобы не мерцало)
        string output = "Score: " + to_string(score) + "\n";
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                if (i == 0 || i == 19 || j == 0 || j == 19) output += "# ";
                else if (i == food.x && j == food.y) output += "$ ";
                else {
                    bool isBody = false;
                    for (auto p : snake.body) {
                        if (p.x == i && p.y == j) {
                            output += "@ ";
                            isBody = true;
                            break;
                        }
                    }
                    if (!isBody) output += "  ";
                }
            }
            output += "\n";
        }
        cout << output;

        Sleep(100); // Скорость игры
    }

    return 0;
}
