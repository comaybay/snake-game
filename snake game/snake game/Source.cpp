#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
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

    string render_str = repeat(width / 2, WALL_TEXTURE + (string)" ");

    for (int i = 2; i < height; i++) {
        render_str += LINE_BREAK + OFFSET_X + WALL_TEXTURE + string(width - 3, ' ') + WALL_TEXTURE;
    }

    render_str += LINE_BREAK + OFFSET_X + repeat(width / 2, WALL_TEXTURE + (string)" ");

    cout << BLUE << render_str << RESET;
}

struct Point
{
    int x, y;
    Point(int x, int y) : x(x), y(y) {};
    Point() : x(-1), y(-1) {};
};


class ConRan
{
private:
    vector<Point> Body;
    Point* head;
    int ChieuDai;
    int huong;
public:
    ConRan(int x0, int y0)
    {   
        if (x0 % 2) x0++;

        Body.reserve(100);
        ChieuDai = 5;
        
        for (int i = 0; i < ChieuDai; i++) {
            Body.emplace_back(x0 + 2 + i * 2, y0 + 1);
        }

        head = &Body.back();
        huong = 0;
    }

    void reset(int x0, int y0)
    {
        if (x0 % 2) x0++;

        Body.clear();
        ChieuDai = 5;

        for (int i = 0; i < ChieuDai; i++) {
            Body.emplace_back(x0 + 2 + i * 2, y0 + 1);
        }
        head = &Body.back();

        huong = 0;
    }
    void Ve() const;
    void DiChuyen();
    const vector<Point>& GetBody() const {
        return Body;
    }
    int getHeadX() const
    {
        return Body.back().x;
    }
    int getHeadY() const
    {
        return Body.back().y;
    }

    void AnDiem(int foodx, int foody);
    bool GameOver();
};

void VeDiem(int x0, int y0, int diem, int level)
{
    gotoxy(x0, y0);
    cout << "Diem: " << diem;
    gotoxy(x0, y0 + 2);
    cout << "Level: " << level;
    gotoxy(x0, y0 + 4);
    cout << "ESC: THOAT";
    gotoxy(x0, y0 + 6);
    cout << "1.TAM DUNG";
    gotoxy(x0, y0 + 8);
    cout << "2.RESET";

}
class Food
{
private:
    Point f;
public:

    int getX() const
    {
        return f.x;
    }
    int getY() const
    {
        return f.y;
    }
    void CreateFood(int x0, int y0, int x, int y, const ConRan& snake);
    void Ve() const;
};

class CollisionChecker {
public:
    static bool isSnakeCollidedWithFood(const ConRan& snake, const Food& food) {
        for (const Point& p : snake.GetBody()) {
            if (food.getX() == p.x && food.getY() == p.y) 
                return true;
        }
        return false;

    }
};

//cac ham cua class ConRan
void ConRan::Ve() const
{
    for (int i = 0; i < ChieuDai - 1; i++)
    {
        gotoxy(Body[i].x, Body[i].y);
        cout << GREEN<< 'o'<<RESET;
    }
    gotoxy(getHeadX(), getHeadY());
    cout <<YELLOW<< '+'<<RESET;
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

    Body[ChieuDai - 2].x = head->x;
    Body[ChieuDai - 2].y = head->y;

    if (huong == 0) head->x += 2;
    else if (huong == 1) head->x -= 2;
    else if (huong == 2) head->y++;
    else if (huong == 3) head->y--;

}

void ConRan::AnDiem(int foodx, int foody)
{
    ChieuDai++;
    Body.emplace_back(head->x, head->y);

    for (int i = ChieuDai - 1; i >= 1; i--) {
        Body[i].x = Body[i - 1].x;
        Body[i].y = Body[i - 1].y;
    }

    head = &Body.back();
}
bool ConRan::GameOver()
{
    for (int i = 0; i < ChieuDai - 1; i++)
    {
        if (Body[i].x == head->x && Body[i].y == head->y)
            return true;
    }
    return false;
}


//Cac ham cua class Food
void Food::CreateFood(int x0, int y0, int x, int y, const ConRan& snake)
{
    do {
    //+2, -4, +1, -2 de random cho ra toa do trong map tro choi (khong tinh vien ben ngoai)
        f.x = random(x0 + 2, x0 + x - 4);
        f.y = random(y0 + 1, y0 + y - 2);
        
        if (f.x % 2) {
            f.x++;
        }

    //neu vi tri moi dung con ran thi doi lai vi tri
    } while (CollisionChecker::isSnakeCollidedWithFood(snake, *this));
}

void Food::Ve() const
{
    gotoxy(f.x, f.y);
    cout <<RED << '*'<<RESET;
}

void RenderScreen(int x0, int y0, int width, int height, int diem, int level, const ConRan& snake, const Food& food) {
    VeManHinh(x0, y0, width, height);
    VeDiem(x0 + width + 5, y0, diem, level);

    snake.Ve();
    food.Ve();

    //dua con tro ra ngoai map
    gotoxy(x0 + width, y0 + height);
}

void ResetGame(int x0, int y0, int width, int height, int& diem, int& level, ConRan& snake, Food& food) {
    diem = 0;
    level = 0;
    snake.reset(x0 + 1, y0 + 1);
    food.CreateFood(x0, y0, width, height, snake);
}

int main()
{
    srand((int)time(0));
    int x0 = 2, y0 = 1, width = 40, height = 20;

    if (x0 % 2) x0++;
    if (width % 2) width++;

    ConRan snake(x0 + 2, y0 + 1);
    Food food;
    int diem = 0, level = 0;
    food.CreateFood(x0, y0, width, height, snake);
    char k;
    while (true)
    {

        if (level < 8)
            level = diem / 3;
        
        RenderScreen(x0, y0, width, height, diem, level, snake, food);

        if (_kbhit())
        {
            k = _getch();
            if (k == 49) system("pause");
            if (k == 50) ResetGame(x0, y0, width, height, diem, level, snake, food);
            if (k == 27) break;
        }

        snake.DiChuyen();
        if (snake.GameOver() || snake.getHeadX() == x0 || snake.getHeadX() == x0 + width - 2 || snake.getHeadY() == y0 || snake.getHeadY() == y0 + height - 1)
        {
            system("cls");
            cout << "\n\t\tGAME OVER";
            cout << "\n\n\n\t\t1.CHOI TIEP";
            cout << "\n\t\tESC.THOAT\n";
            while (1)
            {
                if (_kbhit())
                {
                    k = _getch();
                    if (k == 27) {
                        system("cls");
                        return 0;
                    }

                    if (k == 49) {

                        break;
                    }

                }

                ResetGame(x0, y0, width, height, diem, level, snake, food);
            }

        }

        if (snake.getHeadX() == food.getX() && snake.getHeadY() == food.getY())
        {
            diem++;
            if (diem == 80)
                break;

            snake.AnDiem(food.getX(), food.getY());
            food.CreateFood(x0, y0, width, height, snake);
        }
        
        Sleep(200 - level * 25);
        system("cls");
    }

    cout << "Game Over";
    return 0;
}