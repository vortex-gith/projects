#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <conio.h>

class Tetro
{
    int x, y;
    char figura[4][4];
    bool permissionkey;
    struct Coord { int x, y; };
    std::vector <Coord> memory;
public:
    Tetro(char T[4][4]) : x(0), y(8), permissionkey(true) {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                figura[i][j] = T[i][j];
    }
    void spawn(std::vector <std::vector <char>>& A)
    {

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; ++j) {
                if (figura[i][j] == '@') {
                    if (A[x + i][y + j] == '&')
                        exit(0);
                }
            }
        }

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; ++j) {
                if (figura[i][j] == '@') {
                    A[x + i][y + j] = figura[i][j];
                    memory.push_back({ x + i, y + j });
                }
            }
        }
    }
    bool finished(std::vector <std::vector <char>>& A)
    {
        if (permissionkey == false)
        {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; ++j) {
                    if (figura[i][j] == '@') {
                        A[x + i][y + j] = '&';
                    }
                }
            }

            return true;
        }
        else return false;
    }
    void rotate(std::vector <std::vector <char>>& A, char& key)
    {
        if (key == 'w')
        {

            char temp[4][4];
            int n = 0, m = 3;
            bool canRotate = true;

            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                {
                    temp[i + m - j][j - j + n] = figura[i][j];
                    if (j == 3) { n++; m--; }
                }

            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (temp[i][j] == '@')
                    {
                        int dx = x + i, dy = y + j;
                        if (dx < 0 || dx >= (int)A.size() || dy < 0 || dy >= (int)A[0].size())
                        {
                            canRotate = false; break;
                        }
                        if (A[dx][dy] == '#' || A[dx][dy] == '&')
                        {
                            canRotate = false; break;
                        }
                    }
                }
                if (canRotate == false) break;
            }
            
            if (canRotate == true)
            {
                for (int i = 0; i < memory.size(); i++)
                    A[memory[i].x][memory[i].y] = ' ';
                memory.clear();

                for (int i = 0; i < 4; i++)
                    for (int j = 0; j < 4; j++)
                    {
                        figura[i][j] = temp[i][j];
                    }

                for (int i = 0; i < 4; i++) 
                {
                    for (int j = 0; j < 4; ++j)
                    {
                        if (figura[i][j] == '@')
                        {
                            A[x + i][y + j] = figura[i][j];
                            memory.push_back({ x + i, y + j });
                        }
                    }
                }
            }
        }
    }
    void downfall(std::vector <std::vector <char>>& A)
    {
        int counter = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (figura[i][j] == '@') {
                    int tempx = x + i + 1, tempy = y + j;
                    bool permission = false;
                    for (int k = 0; k < memory.size(); k++)
                    {
                        if (memory[k].x == tempx && memory[k].y == tempy)
                            permission = true;
                    }
                    if (x + i + 1 < A.size() && A[x + i + 1][y + j] != '#' && A[x + i + 1][y + j] != '&' && (permission == true || A[x + i + 1][y + j] == ' '))
                        counter++;
                }
            }
        }
        if (counter == 4)
        {
            for (int i = 0; i < memory.size(); i++)
                A[memory[i].x][memory[i].y] = ' ';
            memory.clear();

            x++;
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                {
                    if (figura[i][j] == '@')
                    {
                        A[x + i][y + j] = figura[i][j];
                        memory.push_back({ x + i, y + j });
                    }
                }
        }

        else permissionkey = false;
    }
    void direction(std::vector <std::vector <char>>& A, char& key)
    {
        if (permissionkey == true)
        {
            if (key == 'a')
            {
                int counter = 0;
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        if (figura[i][j] == '@') {
                            if (y + j - 1 >= 0 && y + j - 1 < A[0].size() && A[x + i][y + j - 1] != '#' && A[x + i][y + j - 1] != '&')
                                counter++;
                        }
                    }
                }
                if (counter == 4)
                {
                    for (int i = 0; i < memory.size(); i++)
                        A[memory[i].x][memory[i].y] = ' ';
                    memory.clear();
                    y--;
                    for (int i = 0; i < 4; i++)
                        for (int j = 0; j < 4; j++)
                        {
                            if (figura[i][j] == '@')
                            {
                                A[x + i][y + j] = figura[i][j];
                            }
                        }
                    for (int i = 0; i < 4; i++)
                        for (int j = 0; j < 4; j++)
                            if (figura[i][j] == '@') memory.push_back({ x + i, y + j });
                }
            }

            if (key == 's')
            {
                int counter = 0;
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        if (figura[i][j] == '@') {
                            if (x + i + 1 >= 0 && x + i + 1 < A.size() && A[x + i + 1][y + j] != '#' && A[x + i + 1][y + j] != '&')
                                counter++;
                        }
                    }
                }
                if (counter == 4)
                {
                    for (int i = 0; i < memory.size(); i++)
                        A[memory[i].x][memory[i].y] = ' ';
                    memory.clear();
                    x++;
                    for (int i = 0; i < 4; i++)
                        for (int j = 0; j < 4; j++)
                        {
                            if (figura[i][j] == '@')
                            {
                                A[x + i][y + j] = figura[i][j];
                            }
                        }
                    for (int i = 0; i < 4; i++)
                        for (int j = 0; j < 4; j++)
                            if (figura[i][j] == '@') memory.push_back({ x + i, y + j });
                }
            }

            if (key == 'd')
            {
                int counter = 0;
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        if (figura[i][j] == '@') {
                            if (y + j + 1 >= 0 && y + j + 1 < A[0].size() && A[x + i][y + j + 1] != '#' && A[x + i][y + j + 1] != '&')
                                counter++;
                        }
                    }
                }
                if (counter == 4)
                {
                    for (int i = 0; i < memory.size(); i++)
                        A[memory[i].x][memory[i].y] = ' ';
                    memory.clear();
                    y++;
                    for (int i = 0; i < 4; i++)
                        for (int j = 0; j < 4; j++)
                        {
                            if (figura[i][j] == '@')
                            {
                                A[x + i][y + j] = figura[i][j];
                            }
                        }
                    for (int i = 0; i < 4; i++)
                        for (int j = 0; j < 4; j++)
                            if (figura[i][j] == '@') memory.push_back({ x + i, y + j });
                }
            }
        }
    }
};

void wait(int& delay)
{
    clock_t start = clock();
    while (((clock() - start) * 1000 / CLOCKS_PER_SEC) < delay);
}

void render(bool Openwindow, std::vector <Tetro>& shapes)
{
    const unsigned int x = 20, y = 20;
    int FPS = 10, delay = 1000 / FPS, score = 0;
    std::vector <std::vector <char>> A(x, std::vector<char>(y));
    int index = rand() % shapes.size();
    Tetro current = shapes[index];

    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
        {
            A[19][j] = '#';
            A[i][19] = '#';
            A[i][0] = '#';
            if (i != 19 && j != 19 && j != 0) A[i][j] = ' ';
        }

    current.spawn(A);

    while (Openwindow == true)
    {
        std::cout << "Score: " << score << std::endl;
        char key = ' ';
        for (int i = 0; i < x; i++)
        {
            for (int j = 0; j < y; j++)
            {
                std::cout << A[i][j];
            }
            std::cout << std::endl;
        }

        if (_kbhit()) key = _getch();

        current.downfall(A);
        current.direction(A, key);
        current.rotate(A, key);

        if (current.finished(A) == true)
        {
            for (int i = 1; i < 19; i++)
            {
                int stroka = 0;
                for (int j = 1; j <= 18; j++)
                {
                    if (A[i][j] == '&') stroka++;
                    if (stroka == 18)
                    {
                        score = score + 180;
                        for (int k = 0; k < y; k++)
                            A[i][k] = ' ';
                        for (int l = i; l > 0; l--)
                            A[l] = A[l - 1];
                        for (int j = 1; j <= 18; j++)
                        {
                            A[1][j] = ' ';
                        }
                        i--;
                    }
                }
            }

            int index = rand() % shapes.size();
            current = shapes[index];
            current.spawn(A);
        }

        wait(delay);
        system("cls");
    }
}

int main()
{
    srand(time(0));

    bool Openwindow = true;
    char T1[4][4] = { ' ', '@', '@', '@',
                     ' ', ' ', '@', ' ',
                     ' ', ' ', ' ', ' ',
                     ' ', ' ', ' ', ' ' };
    char T2[4][4] = { ' ', ' ', '@', ' ',
                     ' ', ' ', '@', ' ',
                     ' ', ' ', '@', ' ',
                     ' ', ' ', '@', ' ' };
    char T3[4][4] = { ' ', '@', '@', ' ',
                     ' ', '@', '@', ' ',
                     ' ', ' ', ' ', ' ',
                     ' ', ' ', ' ', ' ' };
    char T4[4][4] = { ' ', '@', ' ', ' ',
                     ' ', '@', ' ', ' ',
                     ' ', '@', '@', ' ',
                     ' ', ' ', ' ', ' ' };
    char T5[4][4] = { ' ', ' ', '@', ' ',
                     ' ', ' ', '@', ' ',
                     ' ', '@', '@', ' ',
                     ' ', ' ', ' ', ' ' };
    char T6[4][4] = { ' ', ' ', '@', '@',
                     ' ', '@', '@', ' ',
                     ' ', ' ', ' ', ' ',
                     ' ', ' ', ' ', ' ' };
    char T7[4][4] = { ' ', '@', '@', ' ',
                     ' ', ' ', '@', '@',
                     ' ', ' ', ' ', ' ',
                     ' ', ' ', ' ', ' ' };
    std::vector <Tetro> shapes{T1, T2, T3, T4, T5, T6, T7};
    render(Openwindow, shapes);
}