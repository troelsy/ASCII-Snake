//g++ snake.cpp -std=c++11 -o snake && ./snake

#define HEIGHT 24
#define WIDTH 80
#define HOFFSET 10
#define VOFFSET 1
#define SNAKELENGTH 8

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
    resetCursor();

    // Left and right side
    for (int i = 1; i < HEIGHT+1; i++){
        cout << "🌑 🌑 🌑 🌑 🌑";
        moveCursor(i,WIDTH-HOFFSET);
        cout << "🌑 🌑 🌑 🌑 🌑  ";
    }

    // Top and bottom
    moveCursor(1,HOFFSET);
    cout << " 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 ";
    moveCursor(HEIGHT,HOFFSET);
    cout << " 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 🌑 ";
}

void setChar(int row, int column, string c){
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
        SnakeLink(int newx, int newy, int frames);
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
    if (framesLeft == SNAKELENGTH){
        setChar(y,x,"🐸");
    } else {
        setChar(y,x,"❎");
    }
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

    private:
        void spawnLink(int x, int y);

    public:
        void tick();
        void draw();
        void left();
        void right();
        void spawn();
        void destroy();
};

void Snake::left() {
    int t = directionX;
    directionX = -directionY;
    directionY = t;
}

void Snake::right() {
    int t = directionX;
    directionX = directionY;
    directionY = -t;
}

void Snake::spawnLink(int x, int y){
    snakelinks.push_back(new SnakeLink(x, y, SNAKELENGTH));
}

void Snake::spawn(){
    x = 30;
    y = 12;
    directionX = 1;
    directionY = 0;

    spawnLink(x, y);
}

void Snake::draw(){
    for(SnakeLink* link : snakelinks){
        link->draw();
    }

    flushBuffer();
}

bool cleanup(SnakeLink* l){
    return l->dead();
}

void Snake::tick(){
    x = x + directionX;
    y = y + directionY;

    // Decrease life of old links
    for(SnakeLink* link : snakelinks){
        link->tick();
    }

    // Spawn new link
    spawnLink(x, y);

    // Clean up in links, by removing dead ones
    snakelinks.erase(remove_if(snakelinks.begin(), snakelinks.end(), cleanup), snakelinks.end());
}

void Snake::destroy(){
}


class PreySpawner{
    int x;
    int y;

    Snake* snakePointer;

    public:
        static const constexpr char* const prey[] = {"🐭","🐹","🐷","🐢","🐬","🐙"};
        void init(Snake* snakeP);
        void spawn();
        void draw();
        void tick();
};

void PreySpawner::init(Snake* snakeP){
    snakePointer = snakeP;
}

void PreySpawner::spawn(){
    x = rand() % (WIDTH - HOFFSET - HOFFSET) + 1;
    y = rand() % (HEIGHT - VOFFSET - VOFFSET) + 1;
}

void PreySpawner::tick(){
}

void PreySpawner::draw(){
    setChar(y,x,"+");
}


int main(){
    srand (time(NULL)); // Set random seed

    Snake snake;
    snake.spawn();
    snake.draw();

    PreySpawner preyspawner;
    preyspawner.init(&snake);
    preyspawner.spawn();

    int stop = 0;
    int i = 0;
    while (stop != 1){
        clearScreen();
        resetCursor();
        drawFrame();

        preyspawner.tick();
        preyspawner.draw();

        snake.tick();
        snake.draw();

        if (i % 10 == 0){
            snake.right();
        }

        i++;
        usleep(200000); // 200 ms
    }


    moveCursor(25,1);
    return 0;
}