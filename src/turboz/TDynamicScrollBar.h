#ifndef _TDYNAMICSCROLLBAR_H_
#define _TDYNAMICSCROLLBAR_H_

#define Uses_TScrollBar
#include <tv.h>

class TDynamicScrollBar:public TScrollBar{
public:
  TDynamicScrollBar(const TRect& bounds):
    TScrollBar(bounds){}
  
  void handleEvent( TEvent& event );
private:
  int getPartCode();
};


#endif
