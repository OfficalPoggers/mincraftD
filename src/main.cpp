#include <iostream>
#include <stdbool.h>
#include <unistd.h>
#include <string>

#define OLC_PGE_APPLICATION
#include "./olcPixelGameEngine.h"

void run();
void printMap();
void initMap();
void move(int dx, int dy);
void update();
void changeDirection();
void clearScreen();
void generateFood();

char getMapValue(int value);

const int mapwidth  = 20;
const int mapheight = 20;

const int size = mapwidth * mapheight;

int map[size];

int headxpos;
int headypos;
int direction;

int food = 4;

bool running;

class Minecraft : public olc::PixelGameEngine {
public:
  Minecraft() {
    sAppName = "minecraft yes deff minecraft ideed";
  }

private:
  void dead() {
    exit(0);
  }
  
  void run() {
    initMap();
    running = true;
    while (running) {
      changeDirection();
      update();\
      clearScreen();
      printMap();
      // delay 0.4 seconds
      usleep(400000);
    }

    DrawString(0,0, "YOu dead bruh", olc::RED);
    std::cout << "Your score: " << food;
    dead();
  }

  void changeDirection() {
    if (GetKey(olc::Key::W).bPressed) {
      if (direction != 2) direction = 0;
    }
    else if (GetKey(olc::Key::D).bPressed) {
      if (direction != 3) direction = 1;
    }
    else if (GetKey(olc::Key::S).bPressed) {
      if (direction != 4) direction = 2;
    }
    else if (GetKey(olc::Key::A).bPressed) {
      if (direction != 5) direction = 3;
    }
  }

  void move(int dx, int dy){
    int newx = headxpos + dx;
    int newy = headypos + dy;

    if (map[newx + newy * mapwidth] == 2) {
      food++;

      generateFood();
    }
    else if (map[newx + newy * mapwidth] != 0) {
      running = false;
    }

    headxpos = newx;
    headypos = newy;
    map[headxpos + headypos * mapwidth] = food + 1;
  }

  void clearScreen() {
    int x = 0, y = 0;
    do {
      // Generate random x and y values within the map
      x = rand() % (mapwidth - 2) + 1;
      y = rand() % (mapheight - 2) + 1;

      // If location is not free try again
    } while (map[x + y * mapwidth] != 0);
    map[x + y * mapwidth] = -2;
  }

  void update() {
    switch (direction) {
    case 1:
      move(-1, 0);
      break;
    case 2:
      move(0, 1);
      break;
    case 3:
      move(1, 0);
    case 4:
      move(0, -1);
      break;
    }

    // Reduce snake values on map by 1
    for (int i = 0; i < size; i++) {
      if (map[i] > 0) map[i]--;
    }
  }

  void initMap() {
    headxpos = mapwidth / 2;
    headypos = mapheight / 2;
    map[headxpos + headypos * mapwidth] = 1;

    // Places top and bottom walls 
    for (int x = 0; x < mapwidth; ++x) {
      map[x] = -1;
      map[x + (mapheight - 1) * mapwidth] = -1;
    }

    // Places left and right walls
    for (int y = 0; y < mapheight; y++) {
      map[0 + y * mapwidth] = -1;
      map[(mapwidth - 1) + y * mapwidth] = -1;
    }

    // Generates first food
    generateFood();
  }

  void printMap() {
    for (int x = 0; x < mapwidth; ++x) {
      for (int y = 0; y < mapheight; ++y) {
	std::cout << getMapValue(map[x + y * mapwidth]) << std::endl;
      }
    }
  }

  char getMapValue(int value) {
    // Returns a part of snake body
    if (value > 0) return 'o';

    switch (value) {
      // Return wall
    case -1: return 'X';
      // Return food
    case -2: return 'O';
    }
    return 0;
  }

  void generateFood() {
    int x = 0;
    int y = 0;
    do {
      // Generate random x and y values within the map
      x = rand() % (mapwidth - 2) + 1;
      y = rand() % (mapheight - 2) + 1;

      // If location is not free try again
    } while (map[x + y * mapwidth] != 0);

    // Place new food
    map[x + y * mapwidth] = -2;
  }
  
public:
  bool OnUserCreate() override {
    return true;
  }
  
  bool OnUserUpdate(float fElapsedTime) override {
    Clear(olc::BLACK);
    run();
    if (GetKey(olc::Key::Q).bPressed) {
      exit(0);
    }
    return true;
  }
};
 
int main(void) {    
  Minecraft mine;
  if (mine.Construct(400, 300, 1, 1)) {
    mine.Start();
  }
  else{
    std::cout << "ERROR: Cannot construct the screen." << std::endl;
  }
  return 0;
}
