#include <Gamebuino-Meta.h>
#include "graf.h"
#include "map.h"
#include "maps.h"

void debug(int x, int y,int data);

Map::Map()
{ 
  map2=(mapLevel*)malloc(sizeof(maps));
  if(!map2) return;
  levels=sizeof(maps)/sizeof(mapLevel);
  init();
}

void Map::init()
{
  memcpy(map2,maps,sizeof(maps));
  set(0);
}

void Map::set(word num)
{
  level=num;
  draw();
}

void Map::set(byte x,byte y,byte block)
{
  if((y<MAP_Y)&&(x<MAP_X))
    map2[level][y][x]=block;
}

word Map::get()
{
  return level;
}

byte Map::get(byte x,byte y)
{
  if((y<MAP_Y)&&(x<MAP_X))
    return map2[level][y][x];
  return 0;
}

void Map::draw()
  // Dessine tous les blocs composant le niveau
{
  for(byte y=0; y<MAP_Y; y++)
    for(byte x=0; x<MAP_X; x++)
      {
        blocks.setFrame(map2[level][y][x]);
        gb.display.drawImage(x<<3, y<<3, blocks);
      }
}

void Map::draw(bool red)
  // Dessine tous les blocs composant le niveau
{
  for(byte y=0; y<MAP_Y; y++)
    for(byte x=0; x<MAP_X; x++)
      {
        blocks.setFrame((map2[level][y][x]==0)?3:map2[level][y][x]);
        gb.display.drawImage(x<<3, y<<3, blocks);
      }
}

void Map::draw(byte x, byte y)
// Dessine un seul bloc du niveau
{
  if((y<MAP_Y)&&(x<MAP_X))
    {
      blocks.setFrame(map2[level][y][x]);
      gb.display.drawImage(x<<3, y<<3, blocks);
    }
}

bool Map::next()
{
  if(level==levels-1) return false;
  level++;
  draw();
  return true;
}

bool Map::previous()
{
  if(level==0) return false;
  level--;
  draw();
  return true;
}

