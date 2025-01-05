
#include <Gamebuino-Meta.h>
#include "runner.h"
#include "graf.h"
#include "map.h"

void debug(int x, int y,int data);

Runner::Runner(Map *theMap)
{
  map=theMap;
}

void Runner::set(byte x, byte y, bool zero)
{
  posX=x;
  if(y!=255) posY=y;
  anim=0;
  goRight=true; // TODO utiliser goRight et goLeft pour initialiser jumpRight et jumpLeft
  goLeft=false;
  jumpY=0;
  jumpUp=false; // début du saut
  jumpDown=false; // fin du saut
  jumpRight=false; // direction au début du saut
  jumpLeft=false;
  if(zero) blocks=0;
}

runnerStatus Runner::moveRight(bool doJump)
{
  if(posX==80-8) {anim=0; return rightEdge;} // bord d'écran
  if(jumpLeft) return jmp; // pas de changement de direction en cours de saut
  goRight=true;
  goLeft=false;
  if((posX%8)==0) // le joueur va changer de case
  {
    if(doJump) // sauter un niveau plus haut
    {
      if(map->get((posX>>3)+1,(posY>>3)-1)!=0) return block; // bloqué
      if(map->get((posX>>3)+1,posY>>3)!=0) // saut possible
      {
        posY-=8; // saut réalisé
        jumpY=0;
        jumpUp=false;
        jumpDown=false;
        jumpRight=false;
        jumpLeft=false;
      }
    }
    else
      if(map->get((posX>>3)+1,posY>>3)!=0) return block; // bloqué
  }
  posX++;
  anim++;
  if(anim==3) anim=0;
  return ok;
}

runnerStatus Runner::moveLeft(bool doJump)
{
  if(posX==0) {anim=0; return leftEdge;} // bord d'écran
  if(jumpRight) return jmp; // pas de changement de direction en cours de saut
  goRight=false;
  goLeft=true;
  if((posX%8)==0) // le joueur va changer de case
  {
    if(doJump) // sauter un niveau plus haut
    {
      if(map->get((posX>>3)-1,(posY>>3)-1)!=0) return block; // bloqué
      if(map->get((posX>>3)-1,posY>>3)!=0) // saut possible
      {
        posY-=8; // saut réalisé
        jumpY=0;
        jumpUp=false;
        jumpDown=false;
        jumpRight=false;
        jumpLeft=false;

      }
    }
    else
      if(map->get((posX>>3)-1,posY>>3)!=0) return block; // bloqué
  }
  posX--;
  anim++;
  if(anim==3) anim=0;

  return ok;
}

void Runner::stop()
{
  anim=0;
}

runnerStatus Runner::jump()
{
  if(posY<8) return topEdge; // en haut
  if(map->get(posX>>3,(posY>>3)-1)!=0) return block; // bloqué
  if(jumpUp||jumpDown) return ok; // saut en cours
  jumpRight=goRight; // direction au début du saut
  jumpLeft=goLeft; // direction au début du saut
  jumpUp=true; // début du saut
  anim=1;
  return ok;
}

runnerStatus Runner::draw() // dessine le personnage
{
  runnerStatus ret=ok;

  // On efface le sprite avant de le déplacer pour supprimer les traces
  gb.display.setColor(BLACK);
  gb.display.fillRect(posX,posY-jumpY,8,8);

  // Gère le saut du joueur
  if(jumpUp) // montée
  {
//    ret=jmp;
    if(jumpY<8)
    {
      jumpY++;
      anim=1;
    }
    else
    {
      jumpUp=false;
      jumpDown=true;
      ret=top; // haut du saut
    }
  }
  else if(jumpDown) // descente
  {
//    ret=jmp;
    if(jumpY>0)
    {
      jumpY--;
    }
    else // fin du saut
    {
      if(jumpRight)
      {
        if(((posX%8)>3)&&((posX>>3)<(MAP_X-1))) // on replace le joueur pile sur la colonne suivante
          posX=(posX&0xF8)+8;
      }

      if(jumpLeft)
      {
        if(((posX%8)<4)&&((posX>>3)<(MAP_X-1))) // on replace le joueur pile sur la colonne
          posX&=0xF8;
      }
      jumpUp=false;
      jumpDown=false;
      jumpRight=false;
      jumpLeft=false;
    }
  }

  if(!jumpUp&&!jumpDown) // Pas de saut en cours
  {
    if((posY%8)==0) // position pile dans une ligne
    {
      if(posX%8==0) // position pile dans une colonne
      {
        switch(map->get(posX>>3,(posY>>3)+1))
        { 
          case 0 : // case vide dessous
            posY++; // on tombe
            anim=1;
            ret=fall;
            break;
          case 3 : // lave
            gb.display.setColor(BLACK);
            runnerRight.setFrame(anim);
            for(byte i=0; i<8; i++)
            {
              gb.display.drawFastHLine(posX,posY+i,8);
              gb.display.drawImage(posX,posY+i+1,runnerRight);
              setAndDraw((posX>>3),(posY>>3)+1,3,200);
            }
            map->draw(true); // fond rouge
            return ko;
        }
      }
      else // entre deux colonnes
      {
        if((map->get(posX>>3,(posY>>3)+1)==0)&&(map->get((posX>>3)+1,(posY>>3)+1)==0)) // deux cases vides dessous
        { 
          posY++; // on tombe
          anim=1;
          ret=fall;
        }
      }
    }
    else // entre deux lignes
    {
      posY++; // on tombe
      anim=1;
      ret=fall;
    }
  }

  if(goRight)
  {
    runnerRight.setFrame(anim);
    gb.display.drawImage(posX,posY-jumpY,runnerRight);
  }
  else if(goLeft)
  {
    runnerLeft.setFrame(anim);
    gb.display.drawImage(posX,posY-jumpY,runnerLeft);
  }
  return ret;
}

void Runner::setAndDraw(byte x, byte y, byte block, word t)
{
  map->set(x,y,block);
  map->draw(x,y);
  delay(t);
}

void Runner::take() // Prend un bloc
{
  if((posX%8)!=0) return;
  if(goRight&&((posX>>3)<(MAP_X-1)))
  {
    if(map->get((posX>>3)+1,posY>>3)==1) // pierre molle
    {
      setAndDraw((posX>>3)+1,posY>>3,4,100);
      setAndDraw((posX>>3)+1,posY>>3,5,100);
      setAndDraw((posX>>3)+1,posY>>3,6,100);
      setAndDraw((posX>>3)+1,posY>>3,0,0);
      blocks++;
    }
  }
  if(goLeft&&((posX>>3)>0))
  {
    if(map->get((posX>>3)-1,posY>>3)==1) // pierre molle
    {
      setAndDraw((posX>>3)-1,posY>>3,4,100);
      setAndDraw((posX>>3)-1,posY>>3,5,100);
      setAndDraw((posX>>3)-1,posY>>3,6,100);
      setAndDraw((posX>>3)-1,posY>>3,0,0);
      blocks++;
    }
  }
}

void Runner::put() // Pose un bloc
{
  if(!blocks) return;
  switch(posX%8) // tolère plus ou moins un pixel sur la position du joueur
  {
    case 0 : break;
    case 1 : posX--; break;
    case 7 : posX++; break;
    default : return;
  }
  if(goRight&&((posX>>3)<(MAP_X-1)))
  {
    if(map->get((posX>>3)+1,posY>>3)==0) // case vide
    {
      setAndDraw((posX>>3)+1,posY>>3,6,100);
      setAndDraw((posX>>3)+1,posY>>3,5,100);
      setAndDraw((posX>>3)+1,posY>>3,4,100);
      setAndDraw((posX>>3)+1,posY>>3,1,0);
      blocks--;
    }
  }
  if(goLeft&&((posX>>3)>0))
  {
    if(map->get((posX>>3)-1,posY>>3)==0) // case vide
    {
      setAndDraw((posX>>3)-1,posY>>3,6,100);
      setAndDraw((posX>>3)-1,posY>>3,5,100);
      setAndDraw((posX>>3)-1,posY>>3,4,100);
      setAndDraw((posX>>3)-1,posY>>3,1,0);
      blocks--;
    }
  }
}

void Runner::info()
{
  map->draw(0,7);
  map->draw(1,7);
  map->draw(2,7);
  map->draw(3,7);
  gb.display.setFontSize(1);
  gb.display.setColor(YELLOW);
  gb.display.setCursor(1, 57);
  gb.display.print(map->get()+1);
  gb.display.setCursor(17, 57);
  gb.display.print(blocks);

}

