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


TRect Placer::center(const TRect& bounds,int sizex,int sizey){
  TRect r;
  int meanx=(bounds.a.x+bounds.b.x)/2;
  int meany=(bounds.a.y+bounds.b.y)/2;
  r.a.x=meanx-sizex/2;
  r.a.y=meany-sizey/2;
  r.b.x=r.a.x+sizex;
  r.b.y=r.b.y+sizey;
  std::cout<<"returning"<<r.a.x<<" "<<r.a.y<<" ";;
  std::cout<<r.b.x<<" "<<r.b.y<<" "<<std::endl;
  return r;    
}
