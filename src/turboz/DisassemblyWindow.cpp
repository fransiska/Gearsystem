#include "DisassemblyWindow.h"
#include "turbozPalette.h"
#include "TDynamicScrollBar.h"
#define Uses_TScrollBar
#define Uses_TScroller
#define Uses_TPalette
#define Uses_TFrame
#define Uses_TEvent
#include <tv.h>

class DisassemblyScroller:public TScroller{
public:
  DisassemblyScroller( const TRect& bounds, TScrollBar *aHScrollBar,TScrollBar *aVScrollBar );
  virtual void draw();              
  TPalette& getPalette() const;
  void handleEvent(TEvent& event);
};



DisassemblyScroller:: DisassemblyScroller( const TRect& bounds, TScrollBar *aHScrollBar,TScrollBar *aVScrollBar ):
  TScroller( bounds, aHScrollBar, aVScrollBar )
{
    growMode = gfGrowHiX | gfGrowHiY;
    options = options | ofFramed;
    setLimit( 60,0x10000);
}


void DisassemblyWindow::updatePosition(uint16_t addr){
  sprintf(const_cast<char*>(title)+strlen(title)-4,"%04X",addr);
  frame->drawView();
}

/*void DisassemblyWindow::handleEvent(TEvent& event){
  if (event.what==evBroadcast&&event.message.command==cmScrollBarChanged){
    updateTitle(12);
  }
  TWindow::handleEvent(event); 
  }*/

void DisassemblyScroller::handleEvent(TEvent& event){
 if (event.what==evBroadcast){
    switch(event.message.command){
    case cmScrollBarChanged:
      static_cast<DisassemblyWindow*>(owner)-> updatePosition(vScrollBar->value);
      break;
    }//switch
  }//if evCommand
 TScroller::handleEvent(event);
}
 

DisassemblyWindow::DisassemblyWindow(const TRect& bounds,Disassembly& _disassembly):
   TWindowInit( &DisassemblyWindow::initFrame ),
   TWindow( bounds,"Disasm 0000", 0),
   disassembly(_disassembly)  
{
   
  TRect r = getExtent();
  r = TRect( r.b.x-1, r.a.y+1, r.b.x, r.b.y-1 );
  TDynamicScrollBar* vScrollBar;
  insert( vScrollBar = new TDynamicScrollBar(r) );
  vScrollBar->options |= ofPostProcess;

  TScrollBar *hScrollBar =
     standardScrollBar( sbHorizontal |  sbHandleKeyboard );

   r = getClipRect();    // get exposed view bounds
   r.grow( -1, -1 );           // shrink to fit inside window frame
   DisassemblyScroller* scroller=new DisassemblyScroller( r, hScrollBar, vScrollBar );
   insert(scroller);
}


inline char* clipString(char* s,int deltax,int sizex){
  int len=strlen(s);
  if (deltax>len){
    s[0]=0;
  }else{
    s+=deltax;
    s[sizex]=0;
  }
  return s;
}



TPalette& DisassemblyScroller::getPalette() const{
  static TPalette passthrough(0,0);
  return passthrough;
}


TView* getRoot(TView* v){
  while(v->owner!=NULL){
    v=v->owner;
  }
  return v;
}

void DisassemblyScroller::draw(){ 
  static const int BUFFERSIZE=256;
  char buffer[BUFFERSIZE];
  Disassembly& disassembly(static_cast<DisassemblyWindow*>(owner)->getDisassembly());
  
  for( int i = 0; i < size.y; i++ ){   
    int addr = delta.y + i;       // delta is scroller offset          
    TDrawBuffer b;
    b.moveChar( 0, ' ', getColor(1), size.x );// fill line buffer with spaces
    sprintf(buffer,"%04X: %02X ",addr,disassembly.getData(addr));

    Disassembly::State state=disassembly.getState(addr);
    if (state==Disassembly::ConfirmedHead||state==Disassembly::PredictedHead){
      int usedBuffer=strlen(buffer);
      disassembly.disassemble(buffer+usedBuffer,BUFFERSIZE-usedBuffer,addr);
    }
    short color;
    switch(state){
    case Disassembly::PredictedHead:
      color=palette::DARKGRAY;
      break;
    case Disassembly::PredictedTail:
      color=palette::BLACK;
      break;
    case Disassembly::ConfirmedHead:
      color=palette::WHITE;
      break;
    case Disassembly::ConfirmedTail:
      color=palette::GRAY;
      break;
    case Disassembly::Unknown:
      color=palette::MAGENTA;
    }
   
    color=color+palette::BACKGROUND*palette::BLUE;    
    char* s=clipString(buffer,delta.x,size.x);
    
    b.moveStr( 0, s, color );      
    writeLine( 0, i, size.x, 1, b);
  }
}

