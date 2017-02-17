#include "Disassembly.h"
#include <z80ex_dasm.h>

#define NO_FLAGS 0

Disassembly::Disassembly(Memory* _memory):
  memory(_memory){
  for (size_t i=0;i<MEMORYSIZE;i++){
    state[i]=Unknown;
  }
}




Z80EX_BYTE read_callback(Z80EX_WORD addr, void *user_data){
  return static_cast<Memory*>(user_data)->Read(addr);
}

int Disassembly::getInstructionLength(uint16_t addr){
  char buffer[256];
  int t_states;
  return z80ex_dasm(buffer,256,NO_FLAGS,&t_states,&t_states,read_callback,addr,memory);
}


void Disassembly::enter(uint16_t addr){
  state[addr]=ConfirmedHead;
  int bytes=getInstructionLength(addr);
  for (int i=1;i<bytes;i++){
    state[addr+i]=ConfirmedTail;
  }                    
}

void Disassembly::invalidate(uint16_t addr){
  state[addr]=Unknown;
  for(addr=addr+1;state[addr]!=ConfirmedHead;addr++){
    state[addr]=Unknown;
  }
}




Disassembly::State Disassembly::getState(uint16_t addr){
  if (state[addr]==Unknown){
    int a=addr;
    for (; a>0&& state[a-1]==Unknown;a--);
    //here state[a] is the first unknown of the block
    while(a<=addr){
      state[a]=PredictedHead;      
      int bytes=getInstructionLength(a);
      for (int i=1;i<bytes;i++){
        if (a+1<=0xFFFF){
          state[a+i]=PredictedTail;
        }
      }
      a+=bytes;
    }    
  }
  return state[addr];  
}




Disassembly::Info Disassembly::disassemble(char* dis, int disMaxLength, uint16_t addr){
  Disassembly::Info info;
  info.length=z80ex_dasm(dis,disMaxLength,NO_FLAGS,&info.t_states,&info.t_states2,read_callback,addr,memory);
  return info;
}

uint8_t Disassembly::getData(uint16_t addr){
  return memory->Read(addr);
}
