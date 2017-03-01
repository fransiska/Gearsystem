#include "ProcessorWindow.h"
#include "commands.h"

#define Uses_TEvent
#define Used_TView
#include <tv.h>
#include <iostream>
#include <sstream>

class TLightInputLine:public TInputLine{
public:
  TLightInputLine( const TRect& bounds, int aMaxLen)
    :TInputLine(bounds,aMaxLen){
    std::stringstream ss;    
    ss<<"%0"<<(aMaxLen-1)<<"X";
    formatString=ss.str();
  }

  ~TLightInputLine(){

    //std::cout<<"TLightInputLine "<<this<<" destroyed"<<std::endl;
  }

  
  void update(uint16_t v){//assumes len(str)>=maxlen
    sprintf(data,formatString.c_str(),v);
    writeStr(1,0,data,1);

  }
  
private:
  std::string formatString;    
};



ProcessorWindow::Register::Register(EightBitRegister* _h,EightBitRegister* _l,TLightInputLine* _input):
  l(_l),h(_h),input(_input)
{  
}

void ProcessorWindow::Register::update(){
  //std::cout<<"popping data from "<<input<<std::endl;
  if (h){
    uint16_t v=(static_cast<uint16_t>(h->GetValue())<<8)+l->GetValue();   
    input->update(v);
    //std::cout<<"v is"<<v<<std::endl;
  }else{
    uint8_t v=l->GetValue();
    input->update(v);
  }
}



void ProcessorWindow::addRegister(Placer& placer,Monitor::Register h,Monitor::Register l,const char* label){
  bool single=h==NOREGISTER;
  TRect labelbb=placer.place(single?2:3,1);
  TRect inputbb=placer.place(single?4:6,1);

  TLightInputLine* il=new TLightInputLine(inputbb,single?3:5);
  
  registers.push_back
    (
     Register(
              single?NULL:processor.getRegister(h),
              processor.getRegister(l),
              il
              )
     );
  insert(il);
  insert(new TLabel(labelbb,label,il));
  registers.back().update();
}

ProcessorWindow::ProcessorWindow(const TRect& bounds,Processor& _processor):
  TWindowInit( &TDialog::initFrame ),
  TDialog(bounds,"CPU"),
  processor(_processor)
{
  //TWindow( bounds,"Registerm window", 1){  
  Placer p(1,1);
  addRegister(p,NOREGISTER,Monitor::A,"A");
  addRegister(p,NOREGISTER,Monitor::F,"F");  
  p.newLine();
  addRegister(p,NOREGISTER,Monitor::B,"B");
  addRegister(p,NOREGISTER,Monitor::C,"C");
  p.newLine();
  addRegister(p,NOREGISTER,Monitor::D,"D");
  addRegister(p,NOREGISTER,Monitor::E,"E");
  p.newLine();
  addRegister(p,NOREGISTER,Monitor::H,"H");
  addRegister(p,NOREGISTER,Monitor::L,"L");
  p.newLine();
  addRegister(p,Monitor::IX_H,Monitor::IX_L,"IX");
  p.newLine();
  addRegister(p,Monitor::IY_H,Monitor::IY_L,"IY");
  p.newLine();
  addRegister(p,Monitor::SP_H,Monitor::SP_L,"SP");
  p.newLine();
  addRegister(p,Monitor::PC_H,Monitor::PC_L,"PC");
  p.newLine();
  selectNext(false);
}


void ProcessorWindow::handleEvent(TEvent& event){
  if (event.what==evCommand){
    switch(event.message.command){
    case cmClose:
      registers.clear();
      break;  
    }
  }else if (event.what==evBroadcast&&event.message.command==cmRefreshState){
      update();
  }
  TDialog::handleEvent(event);
}

void ProcessorWindow::update(){
  for (auto  r=registers.begin();r!=registers.end();++r){
    r->update();
  }       
}



void ProcessorWindow::draw(){
  //std::cout<<"proc win"<<std::endl;
  update();
  TDialog::draw();
}
