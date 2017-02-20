#include "TDynamicInputLine.h"
#include "commands.h"

#define Uses_TEvent
#define Uses_TKeys
#define Uses_TKeys_Extended
#define Uses_TGroup
#include <tv.h>

TDynamicInputLine::TDynamicInputLine( const TRect& bounds, int aMaxLen, TValidator *aValid ):
  TInputLine(bounds,aMaxLen,aValid)
{
}


void TDynamicInputLine::handleEvent(TEvent& event ){
  bool toNotify=false;
  if (event.what==evKeyDown){
    ushort key=ctrlToArrow(event.keyDown.keyCode);
    switch(key){
    case kbLeft:
    case kbRight:
    case kbHome:
    case kbEnd:
    case kbInsert:
      toNotify=false;
      break;
    default:
      toNotify=true;
    }
  }
  TInputLine::handleEvent(event);
  if (toNotify){
    message( owner, evBroadcast, cmTextEdited, this );
  }
}

