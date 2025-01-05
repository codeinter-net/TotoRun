#ifndef _MAP_
#define _MAP_

#define MAP_X 10
#define MAP_Y 8

typedef byte mapLevel[8][10];

using namespace std;

class Map
{
private:
//    byte map[MAP_Y][MAP_X]; // Niveau en cours de jeu
  mapLevel *map2;
  word level;
  word levels;
public:
  Map();
  void init();
  void set(word level);
  void set(byte x,byte y,byte block);
  word get();
  byte get(byte x,byte y);
  void draw();
  void draw(bool red);
  void draw(byte x, byte y);
  bool next();
  bool previous();
};

#endif

