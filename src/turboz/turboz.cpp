#include "System.h"
#include "ProcessorWindow.h"
#include "ExecutionWindow.h"
#include "DisassemblyWindow.h"
#include "commands.h"
#include "Placer.h"
#define Uses_TApplication
#define Uses_TDeskTop
#define Uses_TSubMenu
#define Uses_TMenuItem
#define Uses_TKeys
#define Uses_TMenuBar
#define Uses_TStatusDef
#define Uses_TStatusLine
#define Uses_TStatusItem
#define Uses_TPalette
#define Uses_RProgram
#include <tv.h>
#include <algorithm>
#include "turbozPalette.h"
#include "Spinner.h"

template<typename Window> class WindowFactory{
public:
  static void addWindow(TGroup* g,System& system);
  static void showWindow(TGroup* g,System& system);
  static TRect initPos;
private:
  static Window* getWindow(System& system);
  static TRect getPos();
};





Placer winPlacer(0,0);
template<> TRect WindowFactory<ProcessorWindow>::initPos(winPlacer.place(15,12,true));
template<> TRect WindowFactory<ExecutionWindow>::initPos(winPlacer.spaceAndPlace(0,1,20,5));
template<> TRect WindowFactory<DisassemblyWindow>::initPos(Placer::rightOf(WindowFactory<ProcessorWindow>::initPos,60,20) );


template<typename Window>
void WindowFactory<Window>::addWindow(TGroup* g,System& system){
  Window* win=getWindow(system);
  g->insert(win);
  ObjectTracker<Window>::objs.push_back(win);
}

template<typename Window>
TRect WindowFactory<Window>::getPos(){
  TRect p(initPos);
  p.move(ObjectTracker<Window>::objs.size(),ObjectTracker<Window>::objs.size());
  return p;
}


template<typename Window>
void WindowFactory<Window>::showWindow(TGroup* g,System& system){
  if (ObjectTracker<Window>::objs.size()){
    g->setCurrent(ObjectTracker<Window>::objs.back(),TGroup::normalSelect);
  }else{
    addWindow(g,system);
  }
}


template<> ProcessorWindow* WindowFactory<ProcessorWindow>::getWindow(System& system){
  return new ProcessorWindow(getPos(),system.processor);
}

template<> ExecutionWindow* WindowFactory<ExecutionWindow>::getWindow(System& system){
  return new ExecutionWindow(getPos(),system);
}

template<> DisassemblyWindow* WindowFactory<DisassemblyWindow>::getWindow(System& system){
  return new DisassemblyWindow(getPos(),system.disassembly);
}




class TurboZ : public TApplication
{
public:
  TurboZ(System& system);
  static TMenuBar * initMenuBar( TRect r );
  static TStatusLine *initStatusLine(TRect r);
  void handleEvent(TEvent& event);
  void remove(TView* view);
  TPalette& getPalette() const;
private:
  template<typename WindowType> void showWindow();
  template<typename WindowType> void addWindow();
  System& system;
  TPalette palette;
  Spinner spinner;
};
/* ---------------------------------------------------------------------- */
/*      class TProgram                                                    */
/*                                                                        */
/*      Palette layout                                                    */
/*          1 = TBackground                                               */
/*       2- 7 = TMenuView and TStatusLine                                 */
/*       8-15 = TWindow(Blue)                                             */
/*      16-23 = TWindow(Cyan)                                             */
/*      24-31 = TWindow(Gray)                                             */
/*      32-63 = TDialog                                                   */
/* ---------------------------------------------------------------------- */
enum TVPalette{
  TBackground=1,
  TMenuView_TextNormal=2,
  TMenuView_TextDisabled=3,
  TMenuView_TextShortcut=4,
  TMenuView_SelectedNormal=5,
  TMenuView_SelectedDisabled=6,
  TMenuView_SelectedShortcut=7,
  TWindowBlue_FramePassive=8,
  TWindowBlue_FrameActive=9,
  TWindowBlue_FrameIcon=10,
  TWindowBlue_ScrollBarPage=11,
  TWindowBlue_ScrollBarReserved=12,
  TWindowBlue_ScrollerNormalText=13,
  TWindowBlue_ScrollerSelectedText=14,
  TWindowBlue_Reserved=15,
  TWindowCyan_FramePassive=16,
  TWindowCyan_FrameActive=17,
  TWindowCyan_FrameIcon=18,
  TWindowCyan_ScrollBarPage=19,
  TWindowCyan_ScrollBarReserved=20,
  TWindowCyan_ScrollerNormalText=21,
  TWindowCyan_ScrollerSelectedText=22,
  TWindowCyan_Reserved=23,
  TWindowGray_FramePassive=24,
  TWindowGray_FrameActive=25,
  TWindowGray_FrameIcon=26,
  TWindowGray_ScrollBarPage=27,
  TWindowGray_ScrollBarReserved=28,
  TWindowGray_ScrollerNormalText=29,
  TWindowGray_ScrollerSelectedText=30,
  TWindowGray_Reserved=31,
  TDialog_FramePassive=32,
  TDialog_FrameActive=33,
  TDialog_FrameIcon=34,
  TDialog_ScrollBarPage=35,
  TDialog_ScrollBarControls=36,
  TDialog_StaticText=37,
  TDialog_LabelNormal=38,
  TDialog_LabelHighlight=39,
  TDialog_LabelShortcut=40,
  TDialog_ButtonNormal=41,
  TDialog_ButtonDefault=42,
  TDialog_ButtonSelected=43,
  TDialog_ButtonDisabled=44,
  TDialog_ButtonShortcut=45,
  TDialog_ButtonShadow=46,
  TDialog_ClusterNormal=47,
  TDialog_ClusterSelected=48,
  TDialog_ClusterShortcut=49,
  TDialog_InputLineNormal=50,
  TDialog_InputLineSelected=51,
  TDialog_InputLineArrows=52,
  TDialog_HistoryArrow=53,
  TDialog_HistorySides=54,
  TDialog_HistoryWindowScrollBarPage=55,
  TDialog_HistoryWindowScrollBarControls=56,
  TDialog_ListViewerNormal=57,
  TDialog_ListViewerFocused=58,
  TDialog_ListViewerSelected=59,
  TDialog_ListViewerDivider=60,
  TDialog_InfoPane=61,
  TDialog_Reserved1=62,
  TDialog_Reserved2=63
};


TurboZ::TurboZ(System& _system) :
  TProgInit( &TurboZ::initStatusLine,
             &TurboZ::initMenuBar,
             &TurboZ::initDeskTop
             ),
  system(_system),
  palette(TProgram::getPalette())
{
  showWindow<ProcessorWindow>();
  addWindow<DisassemblyWindow>();
  showWindow<ExecutionWindow>();
  int background=palette::DARKGRAY;
  int menuback=palette::BLACK;
  palette[TBackground]=palette::BLACK*palette::BACKGROUND+palette::DARKGRAY;
  palette[TMenuView_TextNormal]=menuback*palette::BACKGROUND+palette::CYAN;
  palette[TMenuView_TextDisabled]=menuback*palette::BACKGROUND+palette::DARKGRAY;
  palette[TMenuView_TextShortcut]=menuback*palette::BACKGROUND+palette::CYAN+palette::LIGHT;
  palette[TMenuView_SelectedNormal]=palette::CYAN*palette::BACKGROUND+palette::WHITE;
  palette[TMenuView_SelectedDisabled]=palette::CYAN*palette::BACKGROUND+palette::DARKGRAY;
  palette[TMenuView_SelectedShortcut]=palette::CYAN*palette::BACKGROUND+palette::GRAY; 
  palette[TWindowBlue_FramePassive]=background*palette::BACKGROUND+palette::GRAY;
  palette[TWindowBlue_FrameActive]=background*palette::BACKGROUND+palette::GRAY;
  palette[TWindowBlue_FrameIcon]=background*palette::BACKGROUND+palette::WHITE;
  palette[TWindowBlue_ScrollBarPage]=background*palette::BACKGROUND+palette::GRAY;

  palette[TDialog_FramePassive]= palette[TWindowBlue_FramePassive];
  palette[TDialog_FrameActive]=palette[TWindowBlue_FrameActive];
  palette[TDialog_FrameIcon]=palette[TWindowBlue_FrameIcon];
  palette[TDialog_ScrollBarPage]=palette[TWindowBlue_ScrollBarPage];

  palette[TDialog_LabelNormal]=background*palette::BACKGROUND+palette::WHITE;
  palette[TDialog_LabelHighlight]=background*palette::BACKGROUND+palette::CYAN+palette::LIGHT;

  palette[TDialog_InputLineNormal]=palette::BLACK*palette::BACKGROUND+palette::WHITE;
  palette[TDialog_InputLineSelected]=palette::CYAN*palette::BACKGROUND+palette::WHITE;
  
  palette[TDialog_ButtonShadow]=background*palette::BACKGROUND+palette::BLACK;

  int buttonback=palette::CYAN;
  
  palette[TDialog_ButtonNormal]=buttonback*palette::BACKGROUND+palette::BLUE;
  palette[TDialog_ButtonDefault]=buttonback*palette::BACKGROUND+palette::CYAN;
  palette[TDialog_ButtonSelected]=buttonback*palette::BACKGROUND+palette::WHITE;
  palette[TDialog_ButtonDisabled]=palette::GRAY+palette::CYAN;
  palette[TDialog_ButtonShortcut]=palette[TDialog_ButtonDefault]+palette::LIGHT;
  
  
}

void TurboZ::handleEvent(TEvent& event){
  TApplication::handleEvent(event); 
  if (event.what==evCommand){
    switch(event.message.command){
    case cmShowProcessorWindow:
      showWindow<ProcessorWindow>();
      clearEvent(event);
      break;
    case cmShowExecutionWindow:
      showWindow<ExecutionWindow>();
      clearEvent(event);
      break;
    case cmAddDisassemblyWindow:
      addWindow<DisassemblyWindow>();
      clearEvent(event);
      break;
    case cmStep:
      system.processor.Tick();
      message(this,evBroadcast,cmRefresh,NULL);
      clearEvent(event);
      break;
    }
  }
};

template<typename Window> void TurboZ::showWindow(){
  WindowFactory<Window>::showWindow(deskTop,system);
}

template<typename Window> void TurboZ::addWindow(){
  WindowFactory<Window>::addWindow(deskTop,system);
}




TMenuBar *TurboZ::initMenuBar( TRect r )
{

  r.b.y = r.a.y + 1;    // set bottom line 1 line below top line
  return new TMenuBar
    (
     r,
     *new TSubMenu( "~F~ile", kbAltF )+
     //*new TMenuItem( "~O~pen", cmOpen, kbF3, hcNoContext, "F3" )+
     newLine()+
     *new TMenuItem( "E~x~it", cmQuit, cmQuit, hcNoContext, "" )+
     *new TSubMenu( "~V~iew", kbAltW )+
     *new TMenuItem( "~R~egisters", cmShowProcessorWindow,  cmShowProcessorWindow, hcNoContext, "" )
     );
}



TStatusLine *TurboZ::initStatusLine(TRect r)
{
  r.a.y = r.b.y - 1;     // move top to 1 line above bottom
  return new TStatusLine
    (
     r,
     *new TStatusDef( 0, 0xFFFF ) +
     // set range of help contexts
     *new TStatusItem( 0, kbF10, cmMenu )+
     *new TStatusItem( "~R~eg", kbAltR, cmShowProcessorWindow)+
     *new TStatusItem( "~E~xec", kbAltE, cmShowExecutionWindow)+
     *new TStatusItem( "~D~isasm", kbAltD, cmAddDisassemblyWindow)+
     *new TStatusItem( "~Alt-F3~ Close", kbAltF3, cmClose )
     // define an item
     );
}


TPalette& TurboZ::getPalette() const{
  return const_cast<TPalette&>(palette);
};
 

int main()
{

  System system;
  TurboZ turboz(system);
  turboz.run();
  return 0;
}
