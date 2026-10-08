#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <conio.h>

struct Cell {
    char symbol;
    std::string color;
};

class Tetro {
public:
    int x, y;
    char fig[4][4];
    std::string color;

    Tetro(char f[4][4], std::string c) : x(0), y(8), color(c) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                fig[i][j] = f[i][j];
    }

    bool isValid(int nx, int ny, std::vector<std::vector<Cell>>& A) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (fig[i][j] == '@') {
                    int X = nx + i;
                    int Y = ny + j;

                    if (X < 0 || X >= 20 || Y < 0 || Y >= 20)
                        return false;

                    if (A[X][Y].symbol != ' ')
                        return false;
                }
        return true;
    }

    void draw(std::vector<std::vector<Cell>>& A, char sym) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (fig[i][j] == '@')
                    A[x + i][y + j] = { sym, color };
    }

    void rotate(std::vector<std::vector<Cell>>& A) {
        char temp[4][4];

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                temp[j][3 - i] = fig[i][j];

        draw(A, ' ');

        bool ok = true;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (temp[i][j] == '@') {
                    int X = x + i;
                    int Y = y + j;
                    if (X < 0 || X >= 20 || Y < 0 || Y >= 20 || A[X][Y].symbol != ' ')
                        ok = false;
                }

        if (ok)
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    fig[i][j] = temp[i][j];

        draw(A, '@');
    }
};

void clearLines(std::vector<std::vector<Cell>>& A, int& score) {
    for (int i = 18; i >= 0; i--) {
        bool full = true;
        for (int j = 1; j < 19; j++) {
            if (A[i][j].symbol == ' ') {
                full = false;
                break;
            }
        }

        if (full) {
            score += 100;

            for (int k = i; k > 0; k--) {
                for (int j = 1; j < 19; j++) {
                    A[k][j] = A[k - 1][j];
                }
            }

            i++; // перепроверить строку
        }
    }
}

void wait(int delay) {
    clock_t start = clock();
    while ((clock() - start) * 1000 / CLOCKS_PER_SEC < delay);
}

int main() {
    srand(time(0));

    const int H = 20, W = 20;
    int delay = 100;
    int score = 0;

    std::vector<std::vector<Cell>> A(H, std::vector<Cell>(W, { ' ', "\033[0m" }));

    for (int i = 0; i < H; i++) {
        A[i][0] = { '#', "\033[90m" };
        A[i][W - 1] = { '#', "\033[90m" };
    }
    for (int j = 0; j < W; j++) {
        A[H - 1][j] = { '#', "\033[90m" };
    }

    char T[4][4] = { {' ','@','@','@'},{' ',' ','@',' '},{' ',' ',' ',' '},{' ',' ',' ',' '} };
    char I[4][4] = { {' ',' ','@',' '},{' ',' ','@',' '},{' ',' ','@',' '},{' ',' ','@',' '} };
    char O[4][4] = { {' ','@','@',' '},{' ','@','@',' '},{' ',' ',' ',' '},{' ',' ',' ',' '} };
    char L[4][4] = { {' ','@',' ',' '},{' ','@',' ',' '},{' ','@','@',' '},{' ',' ',' ',' '} };
    char J[4][4] = { {' ',' ','@',' '},{' ',' ','@',' '},{' ','@','@',' '},{' ',' ',' ',' '} };
    char S[4][4] = { {' ',' ','@','@'},{' ','@','@',' '},{' ',' ',' ',' '},{' ',' ',' ',' '} };
    char Z[4][4] = { {' ','@','@',' '},{' ',' ','@','@'},{' ',' ',' ',' '},{' ',' ',' ',' '} };

    std::vector<Tetro> shapes = {
        Tetro(T,"\033[35m"),
        Tetro(I,"\033[36m"),
        Tetro(O,"\033[33m"),
        Tetro(L,"\033[38;5;208m"),
        Tetro(J,"\033[34m"),
        Tetro(S,"\033[32m"),
        Tetro(Z,"\033[31m")
    };

    while (true) {
        Tetro current = shapes[rand() % shapes.size()];
        current.x = 0;
        current.y = 8;

        if (!current.isValid(current.x, current.y, A)) {
            system("cls");
            std::cout << "GAME OVER\nScore: " << score << "\n";
            break;
        }

        while (true) {
            current.draw(A, '@');

            system("cls");
            std::cout << "Score: " << score << "\n\n";

            for (int i = 0; i < H; i++) {
                for (int j = 0; j < W; j++)
                    std::cout << A[i][j].color << A[i][j].symbol << "\033[0m";
                std::cout << "\n";
            }

            if (_kbhit()) {
                char key = _getch();
                current.draw(A, ' ');

                if (key == 'a' && current.isValid(current.x, current.y - 1, A))
                    current.y--;
                if (key == 'd' && current.isValid(current.x, current.y + 1, A))
                    current.y++;
                if (key == 's' && current.isValid(current.x + 1, current.y, A))
                    current.x++;
                if (key == 'w') current.rotate(A);
                if (key == ' ')
                    while (current.isValid(current.x + 1, current.y, A))
                        current.x++;

                current.draw(A, '@');
            }

            current.draw(A, ' ');
            if (current.isValid(current.x + 1, current.y, A)) {
                current.x++;
                current.draw(A, '@');
            }
            else {
                current.draw(A, '@');
                clearLines(A, score);
                break;
            }

            wait(delay);
        }
    }
}