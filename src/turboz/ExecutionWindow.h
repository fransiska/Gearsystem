#ifndef _EXECUTIONWINDOW_
#define _EXECUTIONWINDOW_


#define Uses_TDialog
#define Uses_TButton
#include <tv.h>

class ExecutionWindow:public TDialog{
public:
  ExecutionWindow(const TRect& bounds);
};


#endif



