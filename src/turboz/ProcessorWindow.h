#ifndef _PROCESSORWINDOW_H_
#define _PROCESSORWINDOW_H_

#include "MonitoredData.h"
#include "Processor.h"
#include "Placer.h"
#include "MonitoredData.h"
#define Uses_TRect
#define Uses_TDialog
#define Uses_TInputLine
#define Uses_TLabel

#include <tv.h>

class ProcessorWindow: public TDialog,public MonitoredData{
public:
  ProcessorWindow(const TRect& bounds,Processor& processor);
  //void draw();
  void popData();
  void handleEvent(TEvent& event);
private:
  Processor& processor;

  void addRegister(Placer& p,Monitor::Register h,Monitor::Register l,const char* label);

  class Register:public MonitoredData{
  public:
    Register(EightBitRegister* l,EightBitRegister*h,TInputLine* input);
    EightBitRegister* l,*h;
    TInputLine* input;
    void popData();
  };
  std::vector<Register> registers;
  static const Monitor::Register NOREGISTER=Monitor::REGISTERS;

};

#endif
