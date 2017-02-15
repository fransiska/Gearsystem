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
  return static_cast<Memory*>(user_data)->Retrieve(addr);
}

int Disassembly::getInstructionLength(uint16_t addr){
  char buffer[1];
  int t_states;
  return z80ex_dasm(buffer,1,NO_FLAGS,&t_states,&t_states,read_callback,addr,&memory);
}


void Disassembly::enter(uint16_t addr){
  state[addr]=ConfirmedHead;
  /*int bytes=getInstructionLength(addr);
  for (int i=1;i<bytes;i++){
    state[addr+i]=ConfirmedTail;
    }*/                       
}


uint16_t Disassembly::predictPreviousHead(uint16_t addr){
  uint16_t start=addr;
  for (; addr>0&& state[addr]!=ConfirmedHead /*&& state[addr]!=PredictedHead*/   ;addr--);
  int nextAddr;
  do{
    nextAddr=addr+getInstructionLength(addr);
  }while(nextAddr<start);
  return addr;
}


uint16_t Disassembly::getNextConfirmedHead(uint16_t addr){
  for (; addr<0xFFFF && state[addr]!=ConfirmedHead;addr++);
  return addr;
}

Disassembly::Info Disassembly::disassemble(char* dis, int disMaxLength, uint8_t addr){
  Disassembly::Info info;
  info.length=z80ex_dasm(dis,disMaxLength,NO_FLAGS,&info.t_states,&info.t_states2,read_callback,addr,memory);
  info.state=state[addr];
  return info;
}

