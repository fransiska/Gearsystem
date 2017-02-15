#include "DisassemblyWindow.h"

#define Uses_TScrollBar
#define Uses_TScroller
#include <tv.h>


class DisassemblyScroller:public TScroller{
public:
  DisassemblyScroller( const TRect& bounds, TScrollBar *aHScrollBar,TScrollBar *aVScrollBar );
  virtual void draw();              // override TView::draw
};


DisassemblyScroller:: DisassemblyScroller( const TRect& bounds, TScrollBar *aHScrollBar,TScrollBar *aVScrollBar ):
  TScroller( bounds, aHScrollBar, aVScrollBar )
{
    growMode = gfGrowHiX | gfGrowHiY;
    options = options | ofFramed;
    setLimit( 60,0x10000);
}


DisassemblyWindow::DisassemblyWindow(const TRect& bounds,Disassembly& _disassembly):
   TWindowInit( &DisassemblyWindow::initFrame ),
   TWindow( bounds,"Code disassembly", 0),
   disassembly(_disassembly)  
{
   
   TScrollBar *vScrollBar =
     standardScrollBar( sbVertical | sbHandleKeyboard );
   TScrollBar *hScrollBar =
     standardScrollBar( sbHorizontal |  sbHandleKeyboard );
   TRect r = getClipRect();    // get exposed view bounds
   r.grow( -1, -1 );           // shrink to fit inside window frame
   insert( new DisassemblyScroller( r, hScrollBar, vScrollBar ));
}


void DisassemblyScroller::draw(){
  char buffer[256];
  ushort color = getColor(0x0301);
  for( int i = 0; i < size.y; i++ ){   
    char* s=buffer;
    TDrawBuffer b;
    b.moveChar( 0, ' ', color, size.x );// fill line buffer with spaces
    int j = delta.y + i;       // delta is scroller offset
    sprintf(buffer,"%04X",j);
    
    int len=strlen(buffer);
    
    if (delta.x>len){
      s[0]=0;
    }else{
      s+=delta.x;
      s[size.x]=0;
    }      
    b.moveStr( 0, buffer, color );      
    writeLine( 0, i, size.x, 1, b);
  }
}

