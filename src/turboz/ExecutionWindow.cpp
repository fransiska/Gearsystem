#include "ExecutionWindow.h"
#include "Placer.h"
#include "commands.h"

#include <iostream>
#include <functional>
#include "TurboZ.h"
#include <chrono>
#include <thread>

#define Uses_TCheckBoxes
#define Uses_TSItem
#include <tv.h>

class ToggleButton:public TButton{
public:

  ToggleButton(const TRect& bounds,const char *uTitle,const char* dTitle,ushort aCommand,ushort aFlags); 

  bool isStateUp(){
    return !state_down;
  }

  bool isStateDown(){
    return state_down;
  }

  
  
private:
  void press();
  std::string uTitle,dTitle;
  bool state_down;
};



ToggleButton::ToggleButton(const TRect& bounds,const char * _uTitle,const char* _dTitle,ushort aCommand,ushort aFlags):
  TButton(bounds,_uTitle,aCommand,aFlags),uTitle(_uTitle),dTitle(_dTitle),state_down(false){}
  
void ToggleButton::press(){
  state_down=!state_down;
  if (state_down){
    TButton::press();
  }
  title=state_down?dTitle.c_str():uTitle.c_str();
  drawView();
}


typedef std::pair<ExecutionWindow*,ToggleButton* > ParamType;

int buttonCallback(unsigned int command,void* v){
  ParamType* param=static_cast<ParamType*>(v);
  if (param->second->isStateDown()){
    static Spinner::HaltCondition haltCondition=std::bind(&ToggleButton::isStateUp,param->second);
    param->first->requestRun(&haltCondition);
  }else{
    //the halt condition should stop the execution    
  }
  return cmValid;
}

void printTime(const std::chrono::time_point<std::chrono::high_resolution_clock>& p){

  std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(p.time_since_epoch());
  std::cout<<ms.count()<<std::endl;
}



  ExecutionWindow::ExecutionWindow(const TRect& bounds,System& _sys):
                  TWindowInit( &TDialog::initFrame ),
                  TDialog(bounds,"Execution"),
                  sys(_sys)
                  
{
  Placer p(1,1);

  TView *b = new TCheckBoxes( p.place(13,1,true),
                              new TSItem( "Rea~l~time",0)
                              );
  insert( b );
  p.newLine();
        

  
  insert( new TButton( p.place(8,2), "~S~tep", cmStep,bfDefault ));
  auto run=new ToggleButton( p.place(8,2), "~R~un","S~t~op", cmRun,bfNormal);
  ParamType* param=new ParamType(this,run);
  run->setCallBack(buttonCallback,param);
  insert(run);  


  fastWork.init=[]{};
  fastWork.job=[this]{
    sys.Tick();
  };
  static std::chrono::time_point<std::chrono::high_resolution_clock>  start;
  static int ticks=0;
  realtimeWork.init=[]{
    start= std::chrono::high_resolution_clock::now();
     ticks=0;
  };
  realtimeWork.job=[this]{
    ticks+=sys.Tick();
    
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime=start+
    std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>
    (
     std::chrono::duration<float>(float(ticks)/freq)
     );
    
    std::this_thread::sleep_until
    (
     endTime    
     );
    
    
    //std::this_thread::sleep_for(waitTime);
  };
  freq=3579540;
}



void ExecutionWindow::requestRun(const Spinner::HaltCondition* haltCondition){
  TurboZ::spinner.setWork(&realtimeWork,haltCondition);  
}
