#include "DisassemblyWindow.h"
#include "turbozPalette.h"
#include "TDynamicScrollBar.h"
#define Uses_TScrollBar
#define Uses_TScroller
#define Uses_TPalette
#define Uses_TFrame
#define Uses_TEvent
#define Uses_TKeys
#define Uses_TDialog
#define Uses_TInputLine
#define Uses_TButton
#define Uses_TListBox
#define Uses_TCollection
#include <tv.h>
#include "Placer.h"
#include "commands.h"
#include "TDynamicInputLine.h"


class DisassemblyScroller:public TScroller{
public:
  DisassemblyScroller( const TRect& bounds, TScrollBar *aHScrollBar,TScrollBar *aVScrollBar );
  virtual void draw();              
  TPalette& getPalette() const;
  void handleEvent(TEvent& event);
  void showGoToDialog();
};





DisassemblyScroller:: DisassemblyScroller( const TRect& bounds, TScrollBar *aHScrollBar,TScrollBar *aVScrollBar ):
  TScroller( bounds, aHScrollBar, aVScrollBar )
{
    growMode = gfGrowHiX | gfGrowHiY;
    options = options | ofFramed;
    setLimit( 80,0x10000);
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

class TSearchResults : public TCollection
{
  public:
  TSearchResults() : TCollection(0, 1) {
    shouldDelete=false;
  };
  virtual void *readItem( ipstream& ) { return 0; };
  virtual void writeItem( void *, opstream& ) {};
  void clear(){
    removeAll();
  }

  void push_back(const char* s){
    insert((void*)s);
  }

  const char* operator[](int i){
    return (const char*)at(i);
  }

  size_t size(){
    return count;
  }

};

#define INVALIDHEX -1

class GoToDialog:public TDialog{
public:  
  GoToDialog(const TRect& pos,Symbols& symbols);
  void handleEvent(TEvent& event);
  virtual Boolean valid( ushort command );  
  uint16_t getChoice();//assumes valid returned true
private:
  void setInputLine(const char* s);
  void updateChoices(const char* s);
  long int getHex(); //returns -1 when invalid
  TSearchResults* list;
  TInputLine* inputLine;
  TListBox* listBox;
  Symbols& symbols;
};


long int GoToDialog::getHex(){
  const char* s=static_cast<const char*>(inputLine->getData());
  if (s[0]==0) return INVALIDHEX;
  char* end;
  long int res=strtol(s,&end,16);
  if (*end==0){
    return res;
  }else{
    return INVALIDHEX;
  }
}



 Boolean GoToDialog::valid( ushort command ){
   switch(command){
   case cmCancel:
     return true;
   default:
     return list->size()>0||getHex()!=INVALIDHEX;
   
   }//switch
}

uint16_t GoToDialog::getChoice(){
  int hex=getHex();
  if (hex==INVALIDHEX){
    hex=symbols.getAddress((*list)[listBox->focused]);    
  }
  return static_cast<uint16_t>(hex); 
}

void GoToDialog::handleEvent(TEvent& event){
 if (event.what==evBroadcast){
   switch(event.message.command){
   case cmListItemSelected:
     setInputLine((*list)[listBox->focused]);
     break;
   case cmTextEdited:
     updateChoices((const char*)inputLine->getData());
   }//switch
 }//if evBroadcast
 TDialog::handleEvent(event);
}


 void GoToDialog::setInputLine(const char* s){
   int len=strlen(s);
   int oldlen=inputLine->maxLen;
   inputLine->maxLen=len;
   inputLine->setData((void*)s);
   inputLine->maxLen=oldlen;
 }


void addChoiceCb(const char* s,void* what){
  static_cast<TSearchResults*>(what)->push_back(s);    
}

void GoToDialog::updateChoices(const char* s){
 

  list->clear();
  symbols.getSubstringMatches(s,addChoiceCb,list);
  listBox->newList(list,false);
}
 
GoToDialog::GoToDialog(const TRect& pos,Symbols& _symbols):
    TWindowInit( &TDialog::initFrame ),
    TDialog( pos, "Go to" ),
    symbols(_symbols)
  {
    Placer placer(2,1);
    inputLine=new TDynamicInputLine(placer.place(20,1,true),256);
    insert( inputLine );          
    TRect listRect(placer.place(19,5));
    TScrollBar * scrollb=new TScrollBar(placer.place(1,5,true));
    scrollb->options|=ofPostProcess;
    insert(scrollb);
    listBox=new TListBox(listRect,1,scrollb);
    list = new TSearchResults();
    listBox->newList(list);
    insert(listBox);
    placer.newLine();   
    placer.space(-1);
    insert( new TButton(placer.place(10,2), "~O~K", cmOK,
                    bfDefault ));
    insert( new TButton(placer.place(10,2), "~C~ancel", cmCancel,
                                  bfNormal ));   
    selectNext(false);

  }


void DisassemblyWindow::scrollTo(uint16_t addr){  
  vScrollBar->setValue(addr);
}


void DisassemblyWindow::showGoToDialog(){
  
  GoToDialog *pd = new GoToDialog(Placer::center(owner->getBounds(),24,11),
                                    disassembly.getSymbols()
                                    );
  if( pd ){
    ushort control=static_cast<TGroup*>(owner)->execView( pd );
    if( control != cmCancel ){
      scrollTo(pd->getChoice());
    }
    CLY_destroy( pd );
  }

}


void DisassemblyWindow::handleEvent(TEvent& event){
  if (event.what== evKeyDown  && event.keyDown.keyCode ==kbAltG){
    showGoToDialog();
    clearEvent( event );
  }
  TWindow::handleEvent(event);  
}


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
  insert( vScrollBar = new TDynamicScrollBar(r) );
  vScrollBar->options |= ofPostProcess;

  TScrollBar *hScrollBar =
     standardScrollBar( sbHorizontal |  sbHandleKeyboard );

   r = getClipRect();    // get exposed view bounds
   r.grow( -1, -1 );           // shrink to fit inside window frame
   DisassemblyScroller* scroller=new DisassemblyScroller( r, hScrollBar, vScrollBar );
   insert(scroller);
}


class ClippedString{
public:
  char* s;
  int offset;
  inline void set(char* s,int deltax,int sizex);  
};




void  ClippedString::set(char* buff,int deltax,int sizex){
  int len=strlen(buff);

  if (deltax>len || deltax+sizex < 0 ){
    s=buff;
    offset=0;
    s[0]=0;
  }else if (deltax>0){
    s=buff+deltax;
    offset=0;
  }else{
    s=buff;
    offset=-deltax;
  }
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
  static const int INFOLENGTH=8;
  char buffer[BUFFERSIZE];
  Disassembly& disassembly(static_cast<DisassemblyWindow*>(owner)->getDisassembly());
  Symbols& sym=disassembly.getSymbols();
  
  int labelMaxLength=sym.getLabelMaxLength();
  
  int labelLength;
  ClippedString cs;
  std::string* labelString;
  for( int i = 0; i < size.y; i++ ){   
    int addr = delta.y + i;       // delta is scroller offset          
    TDrawBuffer b;
    b.moveChar( 0, ' ', getColor(1), size.x );// fill line buffer with spaces
    labelString=sym.getLabel(addr);
    if (labelString){
      labelLength=labelString->length();
      memcpy(buffer,labelString->c_str(),std::min(BUFFERSIZE,labelLength));
      memcpy(buffer+labelLength," ",2);
      labelLength++;
    }else{
      labelLength=0;
    }
    sprintf(buffer+labelLength,"%04X %02X ",addr,disassembly.getData(addr));
    labelLength+=INFOLENGTH;
    
      
    Disassembly::State state=disassembly.getState(addr);
    if (state==Disassembly::ConfirmedHead||state==Disassembly::PredictedHead){
      disassembly.disassembleWithSymbols(buffer+labelLength,BUFFERSIZE-labelLength,addr);
    }
    short color;
    switch(state){
    case Disassembly::PredictedHead:
      color=palette::BLUE;
      break;
    case Disassembly::PredictedTail:
      color=palette::CYAN;
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
   
    color=color+palette::BACKGROUND*(getColor(1)/palette::BACKGROUND);    

    cs.set(buffer,-(labelMaxLength+INFOLENGTH-labelLength) +delta.x,size.x);

    
    b.moveStr( cs.offset, cs.s, color );      
    writeLine( 0, i, size.x, 1, b);
  }
}

