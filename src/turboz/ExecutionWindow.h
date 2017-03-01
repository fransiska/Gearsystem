#ifndef _EXECUTIONWINDOW_
#define _EXECUTIONWINDOW_


#define Uses_TDialog
#define Uses_TButton
#include <tv.h>
#include "TrackedObject.h"
#include "System.h"
#include "Spinner.h"
#include <functional>

class ToggleButton;
class ExecutionWindow:public TDialog,public Tracked<ExecutionWindow>{
public:
  ExecutionWindow(const TRect& bounds,System& sys);
 void requestRun(const Spinner::HaltCondition* haltCondition);
private:
  System& sys;
  Spinner spinner;
  Spinner::Work fastWork,realtimeWork;
 
};


#endif



