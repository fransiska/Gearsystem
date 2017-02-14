#ifndef _SYSTEM_H_
#define _SYSTEM_H_


#include "Processor.h"
#include "SegaMemoryRule.h"
#include "Cartridge.h"
#include "DummyIOPorts.h"


class System{
public:
  System();
  void tick();
  Cartridge cartridge; 
  Memory memory;
  SegaMemoryRule rule;
  DummyIOPorts dummyIO;
  Processor processor;


  
};


#endif
