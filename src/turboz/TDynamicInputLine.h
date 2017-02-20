#ifndef _TDYNAMICINPUTLINE_H_
#define _TDYNAMICINPUTLINE_H_

#define Uses_TInputLine
#include <tv.h>
class TDynamicInputLine:public TInputLine{

public:
  TDynamicInputLine(const TRect& bounds, int aMaxLen, TValidator *aValid=NULL );
  void handleEvent(TEvent& event );
};


#endif
