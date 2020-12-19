#include "ctl.h"

#define MAX_THREAD 256
#define MAX_FILESIZE  4096
#define STACK_SIZE 0x1000
#define HEAP_SIZE 0x1000

#define RUNNING 0
#define BLOCK 1
#define READY 2

#define ERR 0
#define OK 1
#define WAIT 2

#define DEBUG 1

void log(char* format, ...){
#if DEBUG
    va_list args;
    va_start(args,format);
    vprintf(format,args);
    va_end(args);
#endif
}


cpu_plus * threads[MAX_THREAD];

void do_init()
{
    for(int i = 0; i < MAX_THREAD; i++){
        threads[i] = NULL;
    }
}

void go(void(*algo)())
{
    (*algo)();
}

size_t create(char * filename, size_t  pr){
    cpu_plus * cp = (cpu_plus *) malloc(sizeof(cpu_plus));

    int i = 0;
    for(i; i < MAX_THREAD; i++){
        if(!threads[i]) break;
    }
    assert(i<MAX_THREAD);
    threads[i] = cp;
    cp->tid = i;

    memset(cp->filename,0,0x20);
    strncpy(cp->filename,filename,0x1f);

    cp->pr = pr;
    cp->st = READY;
    cp->pid = getpid();
    cp->ctime = time(NULL);

    cp->c.r0 = cp->c.r1 = cp->c.r2 = cp->c.r3 = 0;
    cp->c.r4 = cp->c.r5 = cp->c.r6 = cp->c.r7 = 0;
    cp->c.stack = malloc(STACK_SIZE);
    cp->c.heap = malloc(HEAP_SIZE);
    cp->c.sp = cp->c.stack;
    
    int fd = open(filename,0);
    cp->base = (char *)malloc(MAX_FILESIZE+1);
    read(fd, cp->base,MAX_FILESIZE);
    close(fd);
    cp->c.pc = cp->base;
    return cp->tid;
}

void update(int tid, size_t pr){
    threads[tid]->pr = pr;
}

void delete(int tid)
{
    cpu_plus * cp = threads[tid];
    if(!cp) return;
    free(cp->base);
    cp->base = NULL;
    free(cp);
    threads[tid] = NULL;
    return;
}


void show_thread(cpu_plus* cp)
{
    puts("----------------------------");
    printf("name = %s\ntid = %ld\npr = %ld\n", cp->filename, cp->tid, cp->pr);
    puts("----------------------------");
}

void show(){
    for(int i = 0; i < MAX_THREAD; i++){
        cpu_plus * cp = threads[i];
        if(!cp) continue;
        show_thread(cp);
    }
}

void FIFO(){
    for(int i = 0; i < MAX_THREAD; i++){
        cpu_plus * cp = threads[i];
        if(!cp || cp->st!=READY) continue;
        cp->st = RUNNING;
        log("[STT] THREAD %ld\n", cp->tid);
        start(&(cp->c), INT_MAX);
        log("[END] THREAD %ld\n", cp->tid);
        stop(i);
    }
    return;
}

int check_empty()
{
    for(int i = 0; i < MAX_THREAD;i++){
        if(threads[i]) return 0;
    }
    return 1;
}

void RR(int step){
    int idx = 0;
    while(!check_empty()){
        idx = idx % MAX_THREAD;
        cpu_plus * cp = threads[idx];
        int tid = idx;

        if(!cp || cp->st!=READY){
            idx++;
            continue;
        }
        cp->st = RUNNING;
        log("[STT] THREAD %ld\n", cp->tid);
        int res = start(&(cp->c), step); 
        if(res == OK){
            cp->st = READY;
        }
        else if( res == WAIT) cp->st = BLOCK;  // WAIT
        else{
            stop(idx);   // ERR or EXIT
            log("[END] THREAD %ld\n", cp->tid);
        }
        idx++;
    }
}

void PR()
{
    size_t max_pr = 0;
    size_t max_idx = -1;
    while(1)
    {
        cpu_plus * cp = NULL;
        for(int i = 0; i<MAX_THREAD; i++){
            cp = threads[i];
            if(!cp || cp->st != READY) continue;
            if(cp->pr >= max_pr){
                max_idx = i;
            }
        }
        if( max_idx == -1) break;
        cp = threads[max_idx];
        cp->st = RUNNING;
        log("[STT] THREAD %ld\n", cp->tid);
        start(&(cp->c), INT_MAX);
        log("[END] THREAD %ld\n", cp->tid);
        stop(max_idx);
        max_idx = -1;
    }

}

int start(cpu* c, int step){
    for(int i = 0; i<step;i++){
        int res = run(c);
        if(res!=OK) return res;
    }
    return OK;
}

void stop(int tid){
    delete(tid);
}

