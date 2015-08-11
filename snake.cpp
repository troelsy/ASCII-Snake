#define HEIGHT 24
#define WIDTH 80
#define HOFFSET 10
#define VOFFSET 1

#include <vector>
#include <unistd.h>
#include <iostream>
using namespace std;


void clearScreen(){
    cout << "\033[2J";
}

void moveCursor(int row, int column){
    cout << "\033[" << row <<";" << column << "H";
}

void resetCursor(){
    moveCursor(1,1);
}

void drawFrame(){
    moveCursor(1,1);

    // Left and right side
    for (int i = 1; i < HEIGHT+1; i++){
        cout << "##########";
        moveCursor(i,WIDTH-HOFFSET);
        cout << "##########";
    }

    // Top and bottom
    moveCursor(1,HOFFSET);
    cout << "############################################################";
    moveCursor(HEIGHT,HOFFSET);
    cout << "############################################################";
}

void setChar(int row, int column, char c){
    moveCursor(row+VOFFSET, column+HOFFSET);
    cout << c;
}

void flushBuffer(){
    cout << flush;
}

class SnakeLink{
    int x;
    int y;
    int framesLeft;

    public:
        void draw();
        void setPos(int newx, int newy);
        void tick();
        bool dead();
};

SnakeLink::SnakeLink (int newx, int newy, int frames){
    x = newx;
    y = newy;
    framesLeft = frames;
}

void SnakeLink::setPos(int newx, int newy){
    x = newx;
    y = newy;
}

void SnakeLink::draw(){
    setChar(x,y,'+');
}

void SnakeLink::tick(){
    framesLeft--;
}

bool SnakeLink::dead(){
    return framesLeft <= 0;
}


class Snake{
    int length;
    int x;
    int y;
    int directionX;
    int directionY;

    vector<SnakeLink*> snakelinks;

    SnakeLink* a = new SnakeLink;

    public:
        void tick();
        void draw();
        void left();
        void right();
        void spawn();
        void destroy();
};

void Snake::left() {
    directionX = -directionY;
    directionY = directionX;
}

void Snake::right() {
    directionX = directionY;
    directionY = -directionX;
}

void Snake::spawn(){
    x = 12;
    y = 30;

    a->setPos(x,y);

    snakelinks.push_back(new SnakeLink(12,30,4));
    //snakelinks.pushback();


    //usleep(200000); // 200 ms

}

void Snake::draw(){
    a->draw();

    flushBuffer();
}

void Snake::tick(){
    x = x + directionX;
    y = y + directionY;
}

void Snake::destroy(){
}

int main(){
    clearScreen();
    resetCursor();
    drawFrame();

    Snake snake;
    snake.spawn();
    snake.draw();


    int i = 10;
    while (i > 0)
    {
        snake.tick();
        snake.draw();
        usleep(200000); // 200 ms
        i--;
    }

    moveCursor(25,1);
    return 0;
}