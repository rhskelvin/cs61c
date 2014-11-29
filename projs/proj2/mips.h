#ifndef MIPS_H
#define MIPS_H

#include "types.h"

void execute_syscall(Processor *p);

/* see part1.c */
void decode_instruction(Instruction i);

/* see part2.c */
void execute_instruction(Instruction instruction,Processor* processor,Byte *memory);
void store(Byte *memory,Address address,Alignment alignment,Word value);
Word load(Byte *memory,Address address,Alignment alignment);

#endif
