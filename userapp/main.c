/*
 * vim: filetype=c:fenc=utf-8:ts=4:et:sw=4:sts=4
 */
#include <inttypes.h>
#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <sys/mman.h>
#include <syscall.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "graphics.h"
#include <string.h>
#include"controlModule.c"
#include"ssort.c"


extern void *tlsf_create_with_pool(void* mem, size_t bytes);
extern void *g_heap;

/**
 * GCC insists on __main
 *    http://gcc.gnu.org/onlinedocs/gccint/Collect2.html
 */
void __main()
{
    size_t heap_size = 32*1024*1024;
    void  *heap_base = mmap(NULL, heap_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
	g_heap = tlsf_create_with_pool(heap_base, heap_size);
}
/*
结构体定义
*/
struct passArg
{
    int* array;
    int size;
    int lEdge;
};


/**
 * 新线程所执行的函数
 */
void task_foo1(void *pv)
{
    // int i ;
    // for (i = 0; i < 2000; i++)
    // {
    //     printf("priority: 1:%d\n2:%d\n3:%d\n",getpriority(1),getpriority(2),getpriority(3));
    //     printf("O");
    //     sleep(1);
    // }
    
    // printf("Task1 has done!\n");
    // task_exit(0);
    // while(1)
    //     ;
    struct passArg* args = (struct passArg*)pv; 
    bubbleSort(args->array,args->size,args->lEdge);    //调用排序函数
    
    task_exit(0);
    while(1)
    ;
}

// void task_foo2(void *pv)
// {
//     //画出线程优先级进度条
//     int temp1 = 0;int temp2 = 0;
//     while(1)
//     {
//     int rightPrio = getpriority(2);
//     if(temp1 != rightPrio)
//         drawBar(15, rightPrio*20, 410, 580, RGB(0,0,255));
//     temp1 = rightPrio;

    
//     int leftPrio = getpriority(3);
//     if (temp2 != leftPrio)
//         drawBar(15, leftPrio*20, 0, 580, RGB(0,0,255));
//     temp2 = leftPrio;
// }
//     task_exit(0);
//     while(1)
//         ;
// }

void task_foo3(void *pv)
{
    struct passArg* args = (struct passArg*)pv; 
    bubbleSort(args->array,args->size,args->lEdge);    //调用排序函数
    while(1);
    task_exit(0);
}

// void task_foo4(void *pv)
// {
//     //画出线程优先级进度条
//     int temp2 = 0;
//     while(1)
//     {
//     int leftPrio = getpriority(4);
//     if (temp2 != leftPrio)
//         drawBar(15, leftPrio*20, 0, 580, RGB(0,0,255));
//     temp2 = leftPrio;
// }
// task_exit(0);
// while(1)
//     ;

// }

/**
 * 第一个运行在用户模式的线程所执行的函数
 */

void main(void *pv)
{
    printf("task #%d: I'm the first user task(pv=0x%08x)!\r\n",
            task_getid(), pv);

    //TODO: Your code goes here
    init_graphic(0x115);
    // list_graphic_modes();
    unsigned char*stack_foo1;
    unsigned char*stack_foo3;
    unsigned int stack_size = 1024*1024;
    stack_foo1 = (unsigned char*)malloc(stack_size);
    stack_foo3 = (unsigned char*)malloc(stack_size);
    //定义数组
    srand(time(NULL));
    int size =100;
    int* arr= createArray(size);
    struct passArg arg1;
    struct passArg arg2;
    arg1.array = (int*)malloc( size* sizeof(int));
    arg2.array = (int*)malloc( size* sizeof(int));
    memcpy(arg1.array,arr,sizeof(int)*size);
    memcpy(arg2.array,arr,sizeof(int)*size);
    arg1.size = size;
    arg2.size = size;
    arg1.lEdge = 410;
    arg2.lEdge = 20;

    drawBoundary(390,0,20,RGB(80,20,100));
    drawBoundary(0,0,20,RGB(80,20,100));
    int tid_foo1;int tid_foo3;
    tid_foo1 = task_create(stack_foo1+stack_size, &task_foo1, (void*) &arg1);
    tid_foo3 = task_create(stack_foo3+stack_size, &task_foo3, (void*) &arg2);
    // tid_foo2 = task_create(stack_foo2+stack_size, &task_foo2, NULL);
    control(tid_foo1,tid_foo3);
    

    exit_graphic(); //退出图形界面
    task_exit(0);

}

