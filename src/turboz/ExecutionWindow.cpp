#include "ExecutionWindow.h"
#include "Placer.h"
#include "commands.h"

#include <iostream>
#include <functional>


class ToggleButton:public TButton{
public:

  ToggleButton(const TRect& bounds,const char *uTitle,const char* dTitle,ushort aCommand,ushort aFlags); 

  bool isStateUp(){
    return !state_down;

  }
private:
  void press();
  std::string uTitle,dTitle;
  bool state_down;
};



ToggleButton::ToggleButton(const TRect& bounds,const char * _uTitle,const char* _dTitle,ushort aCommand,ushort aFlags):
  TButton(bounds,_uTitle,aCommand,aFlags),uTitle(_uTitle),dTitle(_dTitle),state_down(false){}
  
void ToggleButton::press(){
  std::cout<<"state is"<<state_down<<std::endl;
  state_down=!state_down;
  //if (state_down){
  TButton::press();
  //}
  title=state_down?dTitle.c_str():uTitle.c_str();
  drawView();
}


typedef std::pair<ExecutionWindow*,ToggleButton* > ParamType;

int buttonCallback(unsigned int command,void* v){
  ParamType* param=static_cast<ParamType*>(v);
  static Spinner::HaltCondition haltCondition=std::bind(&ToggleButton::isStateUp,param->second);
  param->first->requestRun(&haltCondition);
  return cmValid;
}

  ExecutionWindow::ExecutionWindow(const TRect& bounds,System& _sys):
                  TWindowInit( &TDialog::initFrame ),
                  TDialog(bounds,"Execution"),
                  sys(_sys)
                  
{
  Placer p(1,2);

  insert( new TButton( p.place(8,2), "~S~tep", cmStep,bfDefault ));
  auto run=new ToggleButton( p.place(8,2), "~R~un","S~t~op", cmRun,bfNormal);
  ParamType* param=new ParamType(this,run);
  run->setCallBack(buttonCallback,param);
   insert(run);
   fastWork.init=[]{};
   fastWork.job=[this]{
     sys.processor.Tick();
   };
}



void ExecutionWindow::requestRun(const Spinner::HaltCondition* haltCondition){
  std::cout<<"request run"<<std::endl;
  spinner.setWork(&fastWork,haltCondition);  
}
