#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdarg.h>

void do_init();
int start();
void stop();
size_t create(char *, size_t);
void update(int, size_t);
void show();
void delete(int);
void FIFO();
void RR();
void PR();
void go(void(*algo)());

typedef struct{
    unsigned char * pc;
    unsigned char * heap;
    unsigned long * stack;
    unsigned long * sp;
    unsigned long r0;
    unsigned long r1;
    unsigned long r2;
    unsigned long r3;
    unsigned long r4;
    unsigned long r5;
    unsigned long r6;
    unsigned long r7;
}cpu;

int run(cpu*);

typedef struct{
    cpu c;
    size_t pr;
    size_t st;
    char * base;
    size_t tid;
    time_t ctime;
    pid_t pid;
    char filename[0x20];
}cpu_plus;
