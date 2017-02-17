#ifndef _DUMMY_IO_PORTS_
#define _DUMMY_IO_PORTS_


class DummyIOPorts:public IOPorts{
  void Reset(){};
  u8 DoInput(u8 port){return 0;}
  void DoOutput(u8 port ,u8 value){}
  
};


#endif
