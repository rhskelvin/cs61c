#include <stdio.h> // for stderr
#include <stdlib.h> // for exit()
#include "mips.h" // for execute_syscall()
#include "types.h"

void execute_instruction(Instruction instruction,Processor* processor,Byte *memory) {
    
    /* YOUR CODE HERE: COMPLETE THE SWITCH STATEMENTS */
    switch(instruction.opcode) {
        case 0x0: // opcode == 0x0(SPECIAL)
            switch(instruction.rtype.funct) {
                case 0x0: // funct == 0x0 (sll)
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rt] << instruction.rtype.shamt;
                    processor->PC += 4;                            
                    break;
                case 0x2: // funct == 0x2 (srl)
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rt] >> instruction.rtype.shamt;
                    processor->PC += 4;                
                    break;
                case 0x3: // funct == 0x3 (sra)
                    processor->R[instruction.rtype.rd] = (Register) 
                                ((int32_t) processor->R[instruction.rtype.rt] >> instruction.rtype.shamt);
                    processor->PC += 4;                
                    break;
                case 0x8: // funct == 0x8 (jr)
                    processor->PC = processor->R[instruction.rtype.rs];                
                    break;                 
                case 0x9: // funct == 0x9 (jalr)
                    { //declaration is not a statement
                        Word temp = (processor->PC+4);
                        processor->PC = processor->R[instruction.rtype.rs];
                        processor->R[instruction.rtype.rd] = temp;                                                    
                    }
                    break;
                case 0xc: // funct == 0xc (SYSCALL)
                    execute_syscall(processor);
                    processor->PC += 4;
                    break;                       
                case 0x10: // funct == 0x10 (mfhi)
                    processor->R[instruction.rtype.rd] = processor->RHI;
                    processor->PC += 4;                
                    break;
                case 0x12: // funct == 0x12 (mflo)
                    processor->R[instruction.rtype.rd] = processor->RLO;
                    processor->PC += 4;                
                    break;
                case 0x18: // funct == 0x18 (mult)
                    {
                        sDouble temp = (sDouble) processor->R[instruction.rtype.rs] * (sDouble) processor->R[instruction.rtype.rs];
                        processor->RLO = (sWord) ((temp << 32) >>32) ;
                        processor->RHI = (sWord) (temp >> 32);
                        processor->PC += 4;
                    }
                    break;
                case 0x19: // funct == 0x19 (multu)
                    {   
                        Double temp = (Double) processor->R[instruction.rtype.rs] * (Double) processor->R[instruction.rtype.rs];
                        processor->RLO = (sWord) ((temp << 32) >>32) ;
                        processor->RHI = (sWord) (temp >> 32);
                        processor->PC += 4;                 
                    }
                    break;
                case 0x21: // funct == 0x21 (addu)
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] + processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x23: // funct == 0x23 (subu)
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] - processor->R[instruction.rtype.rt];               
                    processor->PC += 4;
                    break;
                case 0x24: // funct == 0x24 (AND)
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] & processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;                    
                case 0x25: // funct == 0x25 (or)
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] | processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x26: // funct == 0x26 (xor)
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] ^ processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x27: // funct == 0x27 (nor)
                    processor->R[instruction.rtype.rd] = ~(processor->R[instruction.rtype.rs] | processor->R[instruction.rtype.rt]);
                    processor->PC += 4;
                    break;
                case 0x2a: // funct == 0x2a (slt)
                    processor->R[instruction.rtype.rd] = (int32_t) processor->R[instruction.rtype.rs] < (int32_t) processor->R[instruction.rtype.rt];
                    processor->PC += 4;                   
                    break;
                case 0x2b: // funct == 0x2b (sltu)
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] < processor->R[instruction.rtype.rt];
                    processor->PC += 4;                
                    break;
                default: // undefined funct
                    fprintf(stderr,"%s: pc=%08x,illegal function=%08x\n",__FUNCTION__,processor->PC,instruction.bits);
                    exit(-1);
                    break;
            }
            break;
        case 0x2: // opcode == 0x2 (J)
            processor->PC = ((processor->PC+4) & 0xf0000000) | (instruction.jtype.addr << 2);
            break;
        case 0x3: // opcode == 0x3 (JAL)
            processor->R[31] = (processor->PC+4);
            processor->PC = ((processor->PC+4) & 0xf0000000) | (instruction.jtype.addr << 2);
            break;        
        case 0x4: // opcode == 0x4 (BEQ)
            if (processor->R[instruction.itype.rs] == processor->R[instruction.itype.rt]) {
                processor->PC = (processor->PC+4) + ((sHalf) instruction.itype.imm * 4);
            } else {
                processor->PC += 4;
            }
            break;        
        case 0x5: // opcode == 0x5 (BNE)
            if (processor->R[instruction.itype.rs] != processor->R[instruction.itype.rt]) {
                processor->PC = (processor->PC+4) + ((sHalf) instruction.itype.imm * 4);
            } else {
                processor->PC += 4;
            }
            break;
        case 0x9: // funct == 0x9 (ADDIU)
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] + (sHalf) instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0xa: // funct == 0xa (SLTI)
            processor->R[instruction.rtype.rt] = ((sWord) processor->R[instruction.itype.rs]) < ((sHalf) instruction.itype.imm);
            processor->PC += 4;
            break;
        case 0xb: // funct == 0xb (SLTIU)
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] < ((sHalf) instruction.itype.imm);
            processor->PC += 4;
            break;
        case 0xc: // funct == 0xc (ANDI)
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] & instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0xd: // opcode == 0xd (ORI)
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] | instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0xe: // funct == 0xe (XORI)
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] ^ instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0xf: // funct == 0xf (LUI)
            processor->R[instruction.itype.rt] = instruction.itype.imm << 16;
            processor->PC += 4;
            break;
case 0x20: // funct == 0x20 (LB)
            processor->R[instruction.itype.rt] = (sByte) load(memory, (processor->R[instruction.itype.rs] + (sHalf) instruction.itype.imm), LENGTH_BYTE);
            processor->PC += 4;
            break;
        case 0x21: // funct == 0x21 (LH)
            processor->R[instruction.itype.rt] = (sHalf) load(memory, (processor->R[instruction.itype.rs] + (sHalf) instruction.itype.imm), LENGTH_HALF_WORD);
            processor->PC += 4;
            break;
        case 0x23: // funct == 0x23 (LW)
            processor->R[instruction.itype.rt] = load(memory, (Word)((processor->R[instruction.itype.rs] + (sHalf) instruction.itype.imm)), LENGTH_WORD);
            processor->PC += 4;
            break;
        case 0x24: // funct == 0x24 (LBU)
            processor->R[instruction.itype.rt] = load(memory, (Word)((processor->R[instruction.itype.rs] + (sHalf) instruction.itype.imm)), LENGTH_BYTE);
            processor->PC += 4;
            break;
        case 0x25: // funct == 0x25 (LHU)
            processor->R[instruction.itype.rt] = load(memory, (Word)((processor->R[instruction.itype.rs] + (sHalf) instruction.itype.imm)), LENGTH_HALF_WORD);
            processor->PC += 4;
            break;
        case 0x28: // funct == 0x28 (SB)
            store(memory, (processor->R[instruction.itype.rs] + (sHalf) instruction.itype.imm), LENGTH_BYTE, processor->R[instruction.itype.rt]);
            processor->PC += 4;
            break;
        case 0x29: // funct == 0x28 (SH)
            store(memory, (processor->R[instruction.itype.rs] + (sHalf) instruction.itype.imm), LENGTH_HALF_WORD, processor->R[instruction.itype.rt]);
            processor->PC += 4;
            break;
        case 0x2b: // funct == 0x2b (SW)
            store(memory, (processor->R[instruction.itype.rs] + (sHalf) instruction.itype.imm), LENGTH_WORD, processor->R[instruction.itype.rt]);
            processor->PC += 4;
            break;

        default: // undefined opcode
            fprintf(stderr,"%s: pc=%08x,illegal instruction: %08x\n",__FUNCTION__,processor->PC,instruction.bits);
            exit(-1);
            break;
    }
}


int check(Address address,Alignment alignment) {

    /* YOUR CODE HERE */
    if(address < 1 || address >= MEMORY_SPACE)
        return 0;
    if(alignment == LENGTH_HALF_WORD && address % 2 != 0)
        return 0;
    if(alignment == LENGTH_WORD && address % 4 != 0)
        return 0;
    
    return 1;
}

void store(Byte *memory,Address address,Alignment alignment,Word value) {
    if(!check(address,alignment)) {
        fprintf(stderr,"%s: bad write=%08x\n",__FUNCTION__,address);
        exit(-1);
    }
    
    /* YOUR CODE HERE */
    *(memory + address) = value;

}

Word load(Byte *memory,Address address,Alignment alignment) {
    if(!check(address,alignment)) {
        fprintf(stderr,"%s: bad read=%08x\n",__FUNCTION__,address);
        exit(-1);
    }
    
    /* YOUR CODE HERE */
    switch (alignment){
        case LENGTH_BYTE:
            return (Word) (* ((Byte*)(memory + address)));
            break;
        case LENGTH_HALF_WORD:
            return (Word) (* ((Half*)(memory + address)));
            break;
        default:
            return *((Word*)(memory+address));
    }
    
    // incomplete stub to let "simple" execute
    // (only handles size == SIZE_WORD correctly)
    // feel free to delete and implement your own way
    // return *(Word*)(memory+address);
}
