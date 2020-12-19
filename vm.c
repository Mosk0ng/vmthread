#include <stdio.h>
#include <stdlib.h>
#include "ctl.h"

#define NOP 0x90

#define SUB 0x60
#define ADD 0x61
#define MOVI 0x62
#define XOR 0x63
#define MOV 0x64

#define PR  0x20
#define PS  0x21

#define B   0x30
#define BC  0x31

#define LD  0x40
#define ST  0x41

#define POP 0x80
#define PUSH 0x81

#define HLT 0xFF

#define LOCK 0x10
#define UNL 0x11

int run(cpu* c)
{
    if(*(c->pc) == PR){
        c->pc++;
        printf("R0 is %lu\n", c->r0);
        printf("R1 is %lu\n", c->r1);
        printf("R2 is %lu\n", c->r2);
        printf("R3 is %lu\n", c->r3);
        printf("PC is %p\n", c->pc);
        printf("HEAP is %p\n", c->heap);
        printf("STACK is %p\n", c->stack);
        printf("SP is %p\n", c->sp);
        return 1;
    }
    if(*(c->pc) == B){
        c->pc++;
        long offset = *((long *)c->pc);
        c->pc+=8;
        c->pc+=offset;
        return 1;
    }
    if(*(c->pc) == BC){
        c->pc++;
        unsigned char rx = *((unsigned char*)c->pc);
        c->pc++;
        unsigned char ry = *((unsigned char*)c->pc);
        c->pc++;
        unsigned long * rs = &(c->r0);
        long offset = *((long *)c->pc);
        c->pc+=8;
        if(rs[rx] == rs[ry]){
            c->pc+=offset;
        }
        return 1;
    }
    if(*(c->pc) == PS){
        c->pc++;
        printf("%s",c->pc);
        c->pc+= strlen(c->pc)+1;
        return 1;
    }
    if(*(c->pc) == NOP){
        c->pc++;
        return 1;
    }
    if(*(c->pc) == SUB){
        c->pc++;
        unsigned char rx = *((unsigned char*)c->pc);
        c->pc++;
        unsigned char ry = *((unsigned char*)c->pc);
        c->pc++;
        unsigned long * rs = &(c->r0);
        rs[rx] -= rs[ry];
        return 1;
    }
    if(*(c->pc) == ADD){
        c->pc++;
        unsigned char rx = *((unsigned char*)c->pc);
        c->pc++;
        unsigned char ry = *((unsigned char*)c->pc);
        c->pc++;
        unsigned long * rs = &(c->r0);
        rs[rx] += rs[ry];
        return 1;
    }
    if(*(c->pc) == HLT){
        c->pc++;
        return 0;
    }
    if(*(c->pc) == MOVI){
        c->pc++;
        unsigned char rx = *((unsigned char *)c->pc);
        c->pc++;
        unsigned long * rs = &(c->r0);
        rs[rx] = *((unsigned long *)c->pc);
        c->pc += 8;
        return 1;
    }
    if(*(c->pc) == MOV){
        c->pc++;
        unsigned char rx = *((unsigned char*)c->pc);
        c->pc++;
        unsigned char ry = *((unsigned char*)c->pc);
        c->pc++;
        unsigned long * rs = &(c->r0);
        rs[rx] = rs[ry];
        return 1;
    }
    if(*(c->pc) == LD){
        c->pc++;
        unsigned long * addr = *((unsigned long **)c->pc);
        c->r1 = *addr;
        c->pc += 8;
        return 1;
    }
    if(*(c->pc) == ST){
        c->pc++;
        unsigned long * addr = *((unsigned long **)c->pc);
        *addr = c->r0;
        c->pc += 8;
        return 1;
    }
    if(*(c->pc) == POP){
        c->pc++;
        unsigned long temp = *((unsigned long *)c->sp);
        c->r1 = temp;
        c->sp--;
        return 1;
    }
    if(*(c->pc) == PUSH){
        c->pc++;
        c->sp++;
        *(c->sp) = c->r0;
        return 1;
    }
    if(*(c->pc) == XOR){
        c->pc++;
        unsigned char rx = *((unsigned char *)c->pc);
        c->pc++;
        unsigned long * rs = &(c->r0);
        rs[rx] = 0;
        return 1;
    }
    if(*(c->pc) == LOCK){   // UNLOCK NOUSE
        c->pc++;
        while(*(c->pc)!=UNL){
            int res = run(c);
            if(res != 1) return res;
        }
        c->pc++;
        return 1;
    }
    if(*(c->pc) == UNL){  // WILL NOT HAPPEND
        c->pc++;
        return 1;
    }
    return 0;
}