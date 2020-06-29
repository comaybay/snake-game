#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
int random(int minN, int maxN)
{
    return minN + rand() % (maxN + 1 - minN);
}
void gotoxy(int column, int line)
{
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition(
        GetStdHandle(STD_OUTPUT_HANDLE),
        coord
    );
}
void VeManHinh(int x0, int y0, int width, int height)
{
    for (int i = x0; i <= x0 + width - 1; i++)
    {
        gotoxy(i, y0);
        cout << '#';
    }

    for (int i = x0; i <= x0 + width - 1; i++)
    {
        gotoxy(i, y0 + height - 1);
        cout << '#';
    }

    for (int i = y0; i <= y0 + height - 1; i++)
    {
        gotoxy(x0, i);
        cout << '#';
    }

    for (int i = y0; i <= y0 + height - 1; i++)
    {
        gotoxy(x0 + width - 1, i);
        cout << '#';
    }
}
void VeDiem(int x0, int y0, int diem)
{
    gotoxy(x0, y0);
    cout << "Diem: " << diem;
}
struct Point
{
    int x, y;
};


class ConRan
{
private:
    Point Head, Body[100];
    int ChieuDai;
public:
    ConRan(int a, int b)
    {
        ChieuDai = 4;
        Body[0].x = a + 1; Body[0].y = b + 1;
        Body[1].x = a + 2; Body[1].y = b + 1;
        Body[2].x = a + 3; Body[2].y = b + 1;
        Head.x = a + 4; Head.y = b + 1;

    }
    void Ve();
    void DiChuyen(int huong);
    int getHeadX()
    {
        return Head.x;
    }
    int getHeadY()
    {
        return Head.y;
    }

    void AnDiem(int foodx, int foody);
    bool GameOver();
};


class Food
{
private:
    Point f;
public:

    int getX()
    {
        return f.x;
    }
    int getY()
    {
        return f.y;
    }
    void CreateFood(int a, int b, int c, int d);
    void Ve();
};

//cac ham cua class ConRan
void ConRan::Ve()
{
    for (int i = 0; i < ChieuDai - 1; i++)
    {
        gotoxy(Body[i].x, Body[i].y);
        cout << 'o';
    }
    gotoxy(Head.x, Head.y);
    cout << '+';
}

void ConRan::DiChuyen(int huong)
{
    for (int i = 0; i < ChieuDai - 2; i++)
        Body[i] = Body[i + 1];
    Body[ChieuDai - 2].x = Head.x;
    Body[ChieuDai - 2].y = Head.y;
    if (huong == 0) Head.x++;
    else if (huong == 1) Head.x--;
    else if (huong == 2) Head.y++;
    else if (huong == 3) Head.y--;

}

void ConRan::AnDiem(int foodx, int foody)
{
    ChieuDai++;
    Body[ChieuDai - 2].x = Head.x;
    Body[ChieuDai - 2].y = Head.y;
    Head.x = foodx;
    Head.y = foody;

}
bool ConRan::GameOver()
{
    for (int i = 0; i < ChieuDai - 1; i++)
    {
        if (Body[i].x == Head.x && Body[i].y == Head.y)
            return true;
    }
    return false;
}


//Cac ham cua class Food
void Food::CreateFood(int a, int b, int c, int d)
{
    f.x = random(a + 2, c - 2);
    f.y = random(b + 2, d - 2);
}
void Food::Ve()
{
    gotoxy(f.x, f.y);
    cout << '*';
}
int main()
{
    srand((int)time(0));
    int x0 = 0, y0 = 0, width = 40, height = 20;
    ConRan snake(x0 + 1, y0 + 1);
    Food food;
    int diem = 0;
    char t;
    int huong = 0;
    food.CreateFood(x0, y0, x0 + width - 1, y0 + height - 1);
    while (1)
    {
        VeManHinh(x0, y0, width, height);
        VeDiem(width + 5, y0, diem);

        snake.Ve();
        food.Ve();

        if (_kbhit())
        {
            t = _getch();
            if (t == 77) huong = 0;
            if (t == 75) huong = 1;
            if (t == 80) huong = 2;
            if (t == 72) huong = 3;
        }

        snake.DiChuyen(huong);
        if (snake.GameOver() || snake.getHeadX() == x0 || snake.getHeadX() == x0 + width - 1 || snake.getHeadY() == y0 || snake.getHeadY() == y0 + height - 1)
        {
            system("cls");
            break;
        }

        if (snake.getHeadX() == food.getX() && snake.getHeadY() == food.getY())
        {

            diem++;
            if (diem == 80)
                break;
            snake.AnDiem(food.getX(), food.getY());
            snake.DiChuyen(huong);
            system("cls");


            VeManHinh(x0, y0, width, height);
            VeDiem(width + 5, y0, diem);
            snake.Ve();
            food.CreateFood(x0, y0, width, height);
            food.Ve();

        }


        Sleep(150);
        system("cls");
    }

    cout << "Game Over";
    return 0;
}
//