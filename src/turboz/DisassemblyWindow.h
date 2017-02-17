#ifndef _DISASSEMBLYWINDOW_H_
#define _DISASSEMBLYWINDOW_H_


#define Uses_TWindow
#include <tv.h>
#include "Disassembly.h"
#include "TrackedObject.h"


class DisassemblyScroller;

class DisassemblyWindow: public TWindow,public Tracked<DisassemblyWindow>{
public:
  DisassemblyWindow(const TRect& bounds,Disassembly& disassembly);
  void updatePosition(uint16_t addr);
  Disassembly& getDisassembly(){return disassembly;}
private:
  Disassembly& disassembly;
};


#endif
