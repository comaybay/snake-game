// Chuong trinh chay tren visual studio,
//dung cac phim mui ten tren ban phim de di chuyen con ran
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;


string repeat(unsigned num, const string& input)
{
    string ret;
    ret.reserve(input.size() * num);
    while (num--)
        ret += input;
    return ret;
}
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
    gotoxy(x0, y0);
    const char WALL_TEXTURE = '#';
    const string LINE_BREAK = "\n";
    const string OFFSET_X = string(x0, ' ');

    string render_str = repeat(width/2,  WALL_TEXTURE + (string)" ");
    
    for (int i = 2; i < height; i++) {
        render_str +=  LINE_BREAK + OFFSET_X + WALL_TEXTURE + string(width - 3, ' ') + WALL_TEXTURE;
    }
    
    render_str += LINE_BREAK + OFFSET_X + repeat(width / 2, WALL_TEXTURE + (string)" ");

    cout << render_str;
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
    int huong = 0;
public:
    ConRan(int a, int b)
    {
        if (a % 2) a++;
        
        ChieuDai = 4;
        Body[0].x = a + 2; Body[0].y = b + 1;
        Body[1].x = a + 4; Body[1].y = b + 1;
        Body[2].x = a + 6; Body[2].y = b + 1;
        Head.x = a + 8; Head.y = b + 1;
    }
    void Ve();
    void DiChuyen();
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

void ConRan::DiChuyen()
{
    char t;
    if (_kbhit())
    {
        t = _getch();
        //lay huong moi, neu huong moi nghich voi huong cu, bo qua.
        if (t == 77 && huong != 1) huong = 0;
        if (t == 75 && huong != 0) huong = 1;
        if (t == 80 && huong != 3) huong = 2;
        if (t == 72 && huong != 2) huong = 3;
    }

    for (int i = 0; i < ChieuDai - 2; i++)
        Body[i] = Body[i + 1];

    Body[ChieuDai - 2].x = Head.x;
    Body[ChieuDai - 2].y = Head.y;

    if (huong == 0) Head.x += 2;
    else if (huong == 1) Head.x -= 2;
    else if (huong == 2) Head.y ++;
    else if (huong == 3) Head.y --;

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
    int rx = random(a + 2, c - 2);
    int ry = random(b + 2, d - 2);

    if (rx % 2) {
        rx = (rx == c - 2) ? rx - 1 : rx + 1;
    }

    f.x = rx;
    f.y = ry;
}
void Food::Ve()
{
    gotoxy(f.x, f.y);
    cout << '*';
}


int main()
{
    srand((int)time(0));
    int x0 = 2, y0 = 1, width = 40, height = 20;
    
    if (x0 % 2) x0++;
    if (width % 2) width++;

    ConRan snake(x0 + 1, y0 + 1);
    Food food;
    int diem = 0;
    food.CreateFood(x0, y0, x0 + width - 1, y0 + height - 1);

    while (1)
    {
        VeManHinh(x0, y0, width, height);
        VeDiem(x0 + width + 5, y0, diem);

        snake.Ve();
        food.Ve();

        //dua con tro ra ngoai map
        gotoxy(x0 + width, y0 + height);

        snake.DiChuyen();
        if (snake.GameOver() || snake.getHeadX() == x0 || snake.getHeadX() == x0 + width - 2 || snake.getHeadY() == y0 || snake.getHeadY() == y0 + height - 1)
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
            food.CreateFood(x0, y0, width, height);
        }

        Sleep(150);
        system("cls");
    }

    cout << "Game Over";
    return 0;
}
//
