#ifndef _EXECUTIONWINDOW_
#define _EXECUTIONWINDOW_


#define Uses_TDialog
#define Uses_TButton
#include <tv.h>
#include "TrackedObject.h"

class ExecutionWindow:public TDialog,public Tracked<ExecutionWindow>{
public:
  ExecutionWindow(const TRect& bounds);
};


#endif



