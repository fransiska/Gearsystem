#ifndef _SYSTEM_H_
#define _SYSTEM_H_


#include "Processor.h"
#include "FlexMemoryRule.h"
#include "Cartridge.h"
#include "DummyIOPorts.h"
#include "Disassembly.h"

class System{
public:
  System();
  Cartridge cartridge; 
  Memory memory;
  FlexMemoryRule rule;
  DummyIOPorts dummyIO;
  Processor processor;
  Disassembly disassembly;  
  Symbols symbols;
};


#endif
