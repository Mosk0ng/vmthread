#include <stdio.h>
#include <stdlib.h>
#include "ctl.h"

#define DEBUG 1

void RR1();
void RR5();

void test_VM(){
    mmap((void*)0x1337000,0x1000,0x7,33,-1,0);
    char buf[0x1000];
    int fd = open("test.vm", 0);
    read(fd,buf,0x1000);
    cpu* c = malloc(sizeof(cpu));
    c->pc = buf;
    c->r0 = c->r1 = 0;
    while(run(c)){;}
}

#define RR_NUM 2
void test_VM2(){
    mmap((void*)0x1337000,0x1000,0x7,33,-1,0);
    char buf[0x1000];
    int fd = open("test.vm", 0);
    read(fd,buf,0x1000);
    cpu* c1 = malloc(sizeof(cpu));
    cpu* c2 = malloc(sizeof(cpu));
    c1->pc = buf;
    c1->r0 = c1->r1 = 0;
    c2->pc = buf;
    c2->r0 = c2->r1 = 0;
    while(1){
        int i, j;
        for(i = 0; i < RR_NUM; i++){
            if(!run(c1)) break;
        }
        printf("num = %ld\n",*((unsigned long *)0x1337000));
        for(j = 0; j < RR_NUM; j++){
            if(!run(c2)) break;
        }
        printf("num = %ld\n",*((unsigned long *)0x1337000));
        if(i!=RR_NUM&&j!=RR_NUM) break;
    }
}


#if DEBUG
void test_PC()
{
    mmap((void*)0x1337000,0x1000,0x7,33,-1,0);
    char buf1[0x1000];
    char buf2[0x1000];
    int fd1 = open("pro.vm", 0);
    int fd2 = open("con.vm", 0);
    read(fd1,buf1,0x1000);
    read(fd2,buf2,0x1000);
    cpu* c1 = malloc(sizeof(cpu)); // P
    cpu* c2 = malloc(sizeof(cpu)); // C
    c1->pc = buf1;
    c2->pc = buf2;
    while(1){
        int i, j;
        for(i = 0; i < RR_NUM*2; i++){
            if(!run(c1)) break;
        }
        printf("p = %ld\n",*((unsigned long *)0x1337000));
        for(j = 0; j < RR_NUM; j++){
            if(!run(c2)) break;
        }
        printf("p = %ld\n",*((unsigned long *)0x1337000));
        if(i!=RR_NUM&&j!=RR_NUM) break;
    }
}
#else
void test_PC()
{
    mmap((void*)0x1337000,0x1000,0x7,33,-1,0);
    do_init();
    for(int i = 0; i<10;i++)
        create("con.vm",0);
    for(int i = 0; i<10;i++)
        create("pro.vm",1);
    go(RR5);
}
#endif

void test_ASM()
{
    do_init();
    create("t1.out",0);
    go(FIFO);
}

void test_SHOW()
{
    do_init();
    size_t t0 = create("test.vm",0);
    size_t t1 = create("test.vm",0);
    puts("CREATE");
    show();
    update(t0,1);
    puts("UPDATE");
    show();
    puts("DELETE");
    delete(t0);
    show();
}



//////////////////////////////////

void test_FIFO(){
    mmap((void*)0x1337000,0x1000,0x7,33,-1,0);
    do_init();
    create("test.vm",0);
    create("test.vm",1);
    create("test.vm",2);
    go(FIFO);
}

void RR1()
{
    RR(1);
}

void RR5(){
    RR(5);
}

void RRMAX()
{
    RR(INT_MAX);
}

void test_RR(){
    mmap((void*)0x1337000,0x1000,0x7,33,-1,0);
    do_init();
    create("test.vm",0);
    create("test.vm",1);
    create("test.vm",2);
    go(RR5);
}

void test_PR()
{
    mmap((void*)0x1337000,0x1000,0x7,33,-1,0);
    do_init();
    create("test.vm",0);
    create("test.vm",1);
    create("test.vm",2);
    go(PR);
}

int main(int argc, char *argv[]){
    if(!strcmp(argv[1],"FIFO")){
        test_FIFO();
    }else
        if(!strcmp(argv[1],"RR")){
        test_RR();
    }else
        if(!strcmp(argv[1],"PR")){
        test_PR();
    }else
        if(!strcmp(argv[1],"VM")){
        test_VM();
    }else
        if(!strcmp(argv[1],"VM2")){
        test_VM2();
    }else
        if(!strcmp(argv[1],"PC")){
        test_PC();
    }else
        if(!strcmp(argv[1],"ASM")){
        test_ASM();
    }else
        if(!strcmp(argv[1],"SHOW")){
        test_SHOW();    
    }
}