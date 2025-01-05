#ifndef _RUNNER_
#define _RUNNER_

// Valeurs de retour pour moveRight, moveLeft et draw :
// ok    : déplacement en cours
// leftEdge  : est arrivé à gauche de l'écran
// rightEdge  : est arrivé à droite de l'écran
// topEdge  : est arrivé en haut de l'écran
// block : est bloqué par un bloc
// fall  : est en train de tomber
// jmp   : saut en cours
// top   : est arrivé au sommet d'un saut
// ko    : est tombé dans la lave
// win   : gagné
enum runnerStatus {ok,leftEdge,rightEdge,topEdge,block,fall,jmp,top,ko,win};

using namespace std;

#include "runner.h"
#include "map.h"

class Runner
{
private:
  byte posX, posY, anim, jumpY;
  bool goRight,goLeft; // direction du joueur
  bool jumpUp,jumpDown; // indicateur de saut montant et descendant
  bool jumpRight,jumpLeft; // direction du joueur avant de sauter
  Map *map; // niveau en cours de jeu
  byte blocks;
  void setAndDraw(byte x, byte y, byte block, word timing);
public:
  Runner(Map *map);
  void set(byte x, byte y=255, bool zero=false);
  runnerStatus moveRight(bool doJump=false);
  runnerStatus moveLeft(bool doJump=false);
  void stop();
  runnerStatus jump();
  runnerStatus draw();
  void take();
  void put();
  void info();
};

#endif

