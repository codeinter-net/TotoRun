// TOTORUN
// 2025-01-02

// Type de carte : Gamebuino Meta
// Port : Arduino Zero
// Programmateur : Arduino ISP

#include <Gamebuino-Meta.h>
#include "graf.h"
#include "map.h"
#include "runner.h"

// --------------------------------------------------------------------

void debug(int x, int y,int data)
{
  gb.display.setFontSize(1);
  gb.display.setColor(WHITE,BLACK);
  gb.display.setCursor(x, y);
  gb.display.print(data);
}

Runner* runner;
Map* gameMap;

void newGame()
{
  gameMap->init();
  runner->set(0,0,true);
}

void setup()
{
  gb.begin();
  gameMap=new Map();
  runner=new Runner(gameMap);
  newGame();
}

void loop()
{
  static runnerStatus runStatus=ok;
  while(!gb.update());

  if((runStatus!=ko)&&(runStatus!=win))
  {
    if(runStatus==ok) // déplacement possible
    {
      if (gb.buttons.repeat(BUTTON_RIGHT,0))
        runStatus=runner->moveRight();
      else if (gb.buttons.repeat(BUTTON_LEFT,0))
        runStatus=runner->moveLeft();
      else
        runner->stop();
      if (gb.buttons.pressed(BUTTON_UP))
        runner->jump();
    }
    else if(runStatus==top) // sommet d'un saut
    {
      if (gb.buttons.repeat(BUTTON_RIGHT,0))
        runStatus=runner->moveRight(true);
      else if (gb.buttons.repeat(BUTTON_LEFT,0))
        runStatus=runner->moveLeft(true);
      else
        runner->stop();
    }
    if (gb.buttons.pressed(BUTTON_A))
      runner->take();
    else if (gb.buttons.pressed(BUTTON_B))
      runner->put();
  
    if(runStatus==rightEdge)
    {
      if(gameMap->next())
        runner->set(0);
      else // partie terminée !
      {
        gb.display.setFontSize(2);
        gb.display.setColor(YELLOW);
        gb.display.setCursor(17, 27);
        gb.display.print("WINNER");
        runStatus=win;
        runner->draw();
        return;
      }
    }
    if(runStatus==leftEdge)
    {
      if(gameMap->previous())
        runner->set(80-8);
    }
    runner->info();
    runStatus=runner->draw();
    if(runStatus==ko)
    {
      gb.display.setFontSize(2);
      gb.display.setColor(YELLOW);
      gb.display.setCursor(5, 27);
      gb.display.print("GAME OVER");
    }
  }
  if (gb.buttons.pressed(BUTTON_MENU))
  {
    newGame();
    runStatus=ok;
  }
}

