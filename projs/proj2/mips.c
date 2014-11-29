#include "mips.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "elf.h"

/* WARNING: DO NOT CHANGE THIS FILE.
 YOU PROBABLY DON'T EVEN NEED TO LOOK AT IT... */

// Pointer to simulator memory
Byte *memory = NULL;

void execute_syscall(Processor *p) {
    Register i;
    
    // syscall number is given by $v0 ($2)
    switch(p->R[2]) {
        case 1: // print an integer
            printf("%d",p->R[4]);
            break;
        case 4: // print a string
            for(i=p->R[4];i<MEMORY_SPACE && load(memory,i,LENGTH_BYTE);i++) {
                printf("%c",load(memory,i,LENGTH_BYTE));
            }
            break;
        case 10: // exit
            printf("exiting the simulator\n");
            exit(0);
            break;
        case 11: // print a character
            printf("%c",p->R[4]);
            break;
        default: // undefined syscall
            fprintf(stderr,"%s: illegal syscall number %d\n",__FUNCTION__,p->R[2]);
            exit(-1);
            break;
    }
}
void execute(Processor *processor,int prompt,int print) {
    Instruction instruction;
    
    /* fetch an instruction */
    instruction.bits = load(memory,processor->PC,LENGTH_WORD);
    
    /* interactive-mode prompt */
    if(prompt) {
        if(prompt==1) {
            printf("simulator paused,enter to continue...");
            while(getchar()!='\n');
        }
        printf("%08x: ",processor->PC);
        decode_instruction(instruction);
    }
    
    execute_instruction(instruction,processor,memory);
    
    // enforce $0 being hard-wired to 0
    processor->R[0] = 0;
    
    // print trace
    if(print) {
        int i,j;
        for(i=0;i<8;i++) {
            for(j=0;j<4;j++) {
                printf("r%2d=%08x ",i*4+j,processor->R[i*4+j]);
            }
            puts("");
        }
    }
}










// die loudly if cond is 0
static void demand(int cond,const char* str,...)
{
    if(cond)
        return;
    
    va_list vl;
    va_start(vl,str);
    
    vfprintf(stderr,str,vl);
    fputs("\n",stderr);
    exit(-1);
    
    va_end(vl);
}

// load program "fn" into memory
void begin(uint8_t* mem,size_t memsize,const char* fn,int disasm) {
    int fd,i,j;
    size_t size;
    struct stat s;
    char* buf;
    const Elf32_Ehdr* eh;
    const Elf32_Shdr* sh;
    const Elf32_Phdr* ph;
    
    fd = open(fn,O_RDONLY);
    demand(fd != -1,"Couldn't open executable file %s!",fn);
    
    demand(fstat(fd,&s) != -1,"Couldn't access executable file %s!",fn);
    size = s.st_size;
    
    buf = (char*)mmap(NULL,size,PROT_READ,MAP_PRIVATE,fd,0);
    eh = (const Elf32_Ehdr*)buf;
    demand(buf != MAP_FAILED,"Couldn't read executable file %s!",fn);
    
    close(fd);
    
    demand(size >= sizeof(Elf32_Ehdr) &&
           strncmp((const char*)eh->e_ident,ELFMAG,strlen(ELFMAG)) == 0 &&
           eh->e_ident[EI_CLASS] == ELFCLASS32 &&
           size >= eh->e_phoff + eh->e_phnum*sizeof(Elf32_Ehdr),
           "Invalid executable file %s!",fn);
    
    sh = (const Elf32_Shdr*)(buf+eh->e_shoff);
    for(i = 0; i < eh->e_shnum; i++,sh++) {
        if(disasm) {
            if(sh->sh_type == SHT_PROGBITS && (sh->sh_flags & SHF_EXECINSTR)) {
                for(j = 0; j < sh->sh_size/4; j++) {
                    printf("%08x: ",sh->sh_offset + 4*j);
                    decode_instruction(*(Instruction*)((uint8_t*)buf + sh->sh_offset + 4*j));
                }
            }
        }
    }
    
    ph = (const Elf32_Phdr*)(buf+eh->e_phoff);
    for(i = 0; i < eh->e_phnum; i++,ph++) {
        if(ph->p_type == SHT_PROGBITS && ph->p_memsz) {
            demand(size >= ph->p_offset + ph->p_filesz &&
                   ph->p_vaddr + ph->p_memsz <= memsize,
                   "Invalid executable file %s!",fn);
            memcpy(mem + ph->p_vaddr,(uint8_t*)buf + ph->p_offset,ph->p_filesz);
            memset(mem + ph->p_vaddr+ph->p_filesz,0,ph->p_memsz - ph->p_filesz);
        }
    }
}



int main(int argc,char** argv) {
    /* options */
    int opt_disasm = 0,opt_regdump = 0,opt_interactive = 0;
    
    /* the architectural state of the CPU */
    Processor processor;
    
    /* parse the command-line args */
    int c;
    while((c=getopt(argc,argv,"drit"))!=-1) {
        switch (c) {
            case 'd':
                opt_disasm = 1;
                break;
            case 'r':
                opt_regdump = 1;
                break;
            case 'i':
                opt_interactive = 1;
                break;
            case 't':
                opt_interactive = 2;
                break;
            default:
                fprintf(stderr,"Bad option %c\n",c);
                return -1;
        }
    }
    
    /* make sure we got an executable filename on the command line */
    if(argc<=optind) {
        fprintf(stderr,"Give me an executable file to run!\n");
        return -1;
    }
    
    
    
    /* load the executable into memory */
    assert(memory==NULL);
    memory = calloc(MEMORY_SPACE,sizeof(uint8_t)); // allocate zeroed memory
    
    begin(memory,MEMORY_SPACE,argv[optind],opt_disasm);
    
    /* if we're just disassembling,exit here */
    if(opt_disasm) {
        return 0;
    }
    
    /* initialize the CPU */
    
    
    
    /* initialize pc to 0x1000 */
    processor.PC = 0x1000;
    /* zero out all registers */
    int i;
    for(i=0;i<32;i++) {
        processor.R[i] = 0;
    }
    /* zero out special registers RLO and RHI */
    processor.RHI = 0;
  	processor.RLO = 0;
    
    
    
    
    /* simulate forever! */
    for(;;) {
        execute(&processor,opt_interactive,opt_regdump);
    }
    
    return 0;
}
