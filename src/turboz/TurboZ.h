#ifndef _TURBOZ_H_
#define _TURBOZ_H_

#define Uses_TApplication
#define Uses_TPalette

#include <tv.h>
#include "Spinner.h"
#include <stdint.h>

class TurboZ : public TApplication
{
public:
  TurboZ(System& system);
  static TMenuBar * initMenuBar( TRect r );
  static TStatusLine *initStatusLine(TRect r);
  void handleEvent(TEvent& event);
  void remove(TView* view);
  TPalette& getPalette() const;
  static Spinner spinner;
  void idle();
  void refresh();
private:
  template<typename WindowType> void showWindow();
  template<typename WindowType> void addWindow();
  System& system;
  TPalette palette;
  bool upToDate;
};

#endif
