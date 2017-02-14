#include "Placer.h"

Placer::Placer(int _x,int _y):
  x(_x),y(_y),identx(_x)
  {}

TRect Placer::place(int sizex,int sizey,bool newline){
  TRect r(x,y,x+sizex,y+sizey);
  x=x+sizex;
  if (newline){
    newLine(sizey);
  }
  return r;
}


void Placer::space(int sizex){
  x+=sizex;
}

void Placer::newLine(int sizey){
  y=y+sizey;
  x=identx;
}

