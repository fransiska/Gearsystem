#ifndef _FLEXMEMORYRULE_H_
#define	_FLEXMEMORYRULE_H_

#include "MemoryRule.h"

class FlexMemoryRule : public MemoryRule
{
public:
  FlexMemoryRule(Memory* pMemory, Cartridge* pCartridge);
  virtual u8 PerformRead(u16 address);
  virtual void PerformWrite(u16 address, u8 value);
  virtual void Reset();
};

#endif

