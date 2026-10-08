#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <conio.h>

class Snake;
class Food;
void wait(int& delay);
void render(bool& Openwindow, Snake& snake, Food& apple);

class Snake
{
private:
    int size;
    struct Point { int x, y; };
    std::vector <Point> body, head;
public:
    Snake() : size(1) { head.push_back({ 10,10 }); }
    void spawn(std::vector <std::vector <char>>& A)
    {
        if (A[head[0].x][head[0].y] == '$') grow();

        for (int i = 0; i < head.size(); i++)
            A[head[i].x][head[i].y] = '@';

        for (int i = 0; i < body.size(); i++)
            if (size > 1) A[body[i].x][body[i].y] = '@';
    }
    void grow() { size++; }
    void breaking(std::vector <std::vector <char>>& A)
    {
        if (A[head[0].x][head[0].y] == '#') exit(0);
        for (int i = 0; i < body.size(); i++)
            if (head[0].x == body[i].x && head[0].y == body[i].y) exit(0);
    }
    void moveSnake(std::vector <std::vector <char>>& A, char& key, char& lastkey)
    {
        if (head.size() <= size)
        {
            body.insert(body.begin(), { head[1].x, head[1].y });
            if (body.size() > size - 1) body.pop_back();
        }
        head.pop_back();
        if (size > 1) lastkey = key;
    }
    void readKey(std::vector <std::vector <char>>& A, char& key, char& lastkey)
    {
        int temphead = 0;

        if ((key == 'w' && lastkey == 's') ||
            (key == 'a' && lastkey == 'd') ||
            (key == 's' && lastkey == 'w') ||
            (key == 'd' && lastkey == 'a') || 
            (key != 'w' && key != 'a' && key != 's' && key != 'd'))
            key = lastkey;

       if (key == 'w')
       {
           temphead = head[0].x - 1;
           head.insert(head.begin(), { temphead, head[0].y });
           moveSnake(A, key, lastkey);
       }
       if (key == 'a')
       {
           temphead = head[0].y - 1;
           head.insert(head.begin(), { head[0].x, temphead });
           moveSnake(A, key, lastkey);
       }
       if (key == 's')
       {
           temphead = head[0].x + 1;
           head.insert(head.begin(), { temphead, head[0].y });
           moveSnake(A, key, lastkey);
       }
       if (key == 'd')
       {
           temphead = head[0].y + 1;
           head.insert(head.begin(), { head[0].x, temphead });
           moveSnake(A, key, lastkey);
       }
    }
};
class Food
{
private:
    int x, y;
public:
    Food() : x((rand() % 18) + 1), y((rand() % 18) + 1) {}
    void spawn(std::vector <std::vector <char>>& A, Snake& snake, int& score)
    {
        if (A[x][y] == '@') { snake.grow(); score++; }

        while (A[x][y] == '@' || A[x][y] == '#')
        {
            x = (rand() % 18) + 1;
            y = (rand() % 18) + 1;
        }
        A[x][y] = '$';
    }
};
void wait(int& delay)
{
    clock_t start = clock();
    while ((clock() - start) * 1000 / CLOCKS_PER_SEC < delay);
}
void render(bool& Openwindow, Snake& snake, Food& apple)
{
    const unsigned int x = 20, y = 20;
    int FPS = 20, delay = 1000 / FPS, score = 0;
    std::vector <std::vector <char>> A(x, std::vector <char>(y));
    char key = ' ', lastkey = ' ';

    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
        {
            A[0][j] = '#';
            A[i][0] = '#';
            A[19][j] = '#';
            A[i][19] = '#';
        }
    while (Openwindow == true)
    {
        std::cout << "Score: " << score << std::endl;

        for (int i = 0; i < x; i++) 
            for (int j = 0; j < y; j++)
                if (i != 19 && j != 19 && i != 0 && j != 0) A[i][j] = ' '; 

        if (_kbhit()) key = _getch();

        snake.spawn(A);
        snake.readKey(A, key, lastkey);
        snake.breaking(A);
        apple.spawn(A, snake, score);

        for (int i = 0; i < x; i++) 
        {
            for (int j = 0; j < y; j++)
            {
                std::cout << " " << A[i][j];
            }
            std::cout << std::endl;
        }

        wait(delay);
        system("cls");
    }
}
int main()
{
    setlocale(0, "");
    srand(time(0));
    bool Openwindow = true;
    Snake snake;
    Food apple;
    render(Openwindow, snake, apple);
}
