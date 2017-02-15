#include "System.h"

System::System():
  rule(&memory,&cartridge),
  processor(&memory),
  disassembly(&memory)
{
  cartridge.LoadFromFile("/home/fabio/dev/emu/mapper.sg");
  memory.Init();
  memory.SetCurrentRule(&rule);
  processor.SetIOPorts(&dummyIO);
}
 
