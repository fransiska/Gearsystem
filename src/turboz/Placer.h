#ifndef _PLACER_H_
#define _PLACER_H_

#define Uses_TRect
#include <tv.h>

class Placer{
  int x,y;
  int identx;
public:
  Placer(int startx=0,int starty=0);
  void setIdentX(int x){identx=x;}
  TRect place(int sizex,int sizey,bool newline=false);
  void newLine(int sizey=1);
  void space(int sizex);
};



#endif
