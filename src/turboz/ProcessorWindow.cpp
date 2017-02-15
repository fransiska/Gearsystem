#include "ProcessorWindow.h"
#include "commands.h"

#define Uses_TEvent
#include <tv.h>
#include <iostream>

ProcessorWindow::Register::Register(EightBitRegister* _h,EightBitRegister* _l,TInputLine* _input):
  l(_l),h(_h),input(_input)
{  
}

void ProcessorWindow::Register::popData(){
  if (h){
    uint16_t v=(static_cast<uint16_t>(h->GetValue())<<8)+l->GetValue();
    char buffer[5];
    sprintf(buffer,"%04X",v);
    input->setData(buffer);
  }else{
    uint8_t v=l->GetValue();
    char buffer[3];
    sprintf(buffer,"%02X",v);
    input->setData(buffer);
  }
}

void ProcessorWindow::addRegister(Placer& placer,Monitor::Register h,Monitor::Register l,const char* label){
  bool single=h==NOREGISTER;
  TRect labelbb=placer.place(single?2:3,1);
  TRect inputbb=placer.place(single?4:6,1);

  TInputLine* il=new TInputLine(inputbb,single?3:5);
  
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
  registers.back().popData();
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


void ProcessorWindow::popData(){
  for (std::vector<Register>::iterator   r=registers.begin();r!=registers.end();++r){
    r->popData();
  }
       
}


void ProcessorWindow::handleEvent(TEvent& event){
  TDialog::handleEvent(event); 
  if (event.what==evBroadcast&&event.message.command==cmRefresh){
    popData();
  }
}
