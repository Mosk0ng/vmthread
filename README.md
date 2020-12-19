# vmthread

vmthread是一个运行字节码的线程库，我在学校的OS作业时完成了他。



## Usage



### 编译程序

你可以下载`libvmthread.so`，把它放在你的`/usr/lib`文件夹下,然后在编译程序的使用使用`-lvmthread`即可。

如：

```shell
gcc test.c -o test -lvmthread
```

或者你可以简单地对源码进行编译：

```shell
gcc -o test test.c vm.c ctl.c ctl.h
```

但是别忘了在源码中`include "ctl.h"`



### 生成字节码

可以通过`asm.py`这个简单的汇编器来生成字节码.

`python asm.py t1.asm t1.out`

```asm
// t1.asm
XOR R0
XOR R1
MOVI R0 100
MOVI R1 200
ADD R0 R1
PR

//t1.out
$ hexdump t1.out
0000000 0063 0163 0062 0100 0000 0000 0000 0162
0000010 0200 0000 0000 0000 0061 2001          
000001c
```



关于字节码的定义可以参照vm.c中的宏定义和代码。



可以使用`gen.py`生成测试所需要的一些字节码程序。

- test.vm

   对一个共享变量循环加1

- con.vm 

  pro.vm

  生产者消费者模型，设置最大容量为5.



### TEST

test为编译好的二进制程序，可以用来测试各种功能是否正常。

```
./test [options]
- FIFO
- RR
- PR
- VM
- VM2
- PC
- ASM
- SHOW
```





## API

所有的你可能会使用到的API都在头文件中，但是不是所有你都会使用到，重要的有这几个：

```c
void do_init();		// 初始化线程池
size_t create(char * filename, size_t pr); // 从文件创建线程，并设置优先级，返回tid
void update(int, size_t);	// 更新优先级，第一个参数是线程tid
void show();		// 显示线程池中的线程信息
void delete(int);	// 删除一个线程
void FIFO();		// 先到先服务算法
void RR();			// 时间片轮转
void PR();			// 优先级调度算法
void go(void(*algo)());			// 运行线程
```



## Demo

```c
#include "ctl.h"
#include <stdio.h>

void test_PC()
{
    mmap((void*)0x1337000,0x1000,0x7,33,-1,0);
    do_init();
    for(int i = 0; i<10;i++)
        create("con.vm",0);		// 写好的字节码文件
    for(int i = 0; i<10;i++)
        create("pro.vm",1);
    go(RR5);
}

int main()
{
    test_PC();
}
```



## TODO



C到字节码的编译器