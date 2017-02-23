#include "ExecutionWindow.h"
#include "Placer.h"
#include "commands.h"

#include <iostream>


ExecutionWindow::ExecutionWindow(const TRect& bounds):
  TWindowInit( &TDialog::initFrame ),
  TDialog(bounds,"Execution")
{
  Placer p(1,2);

  insert( new TButton( p.place(8,2), "~S~tep", cmStep,bfDefault ));
  insert( new TButton( p.place(7,2), "~R~un", cmRun,bfNormal ));
  //insert( new TButton( p.place(7,2), "S~t~op", cmStop,bfNormal ));
  //insert( new TButton( p.place(7,2), "S~t~op", cmStop,bfNormal ));
}
