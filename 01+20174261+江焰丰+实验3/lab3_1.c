#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define thread_number 20

int thread_count = 0;

typedef struct VirtualPCB
{
    int t_id;        //线程号
    int arrive_time; //线程创建的时间
    int need_time;   //线程运行所需的时间
    int t_state;     //线程的状态，0表示没有运行完，1表示运行完
    int priority;    //表示线程的优先级
    int start_time;  //线程开始运行的时间
    int run_time;    //线程当前运行的时间
    int end_time;    //线程结束运行的时间
    int wait_time;   //线程等待时间
} PCB;

PCB PCBS[thread_number];
int run_random[thread_number];
int priority_random[thread_number];

/*
***
随机生成每个进程的运行时间
***
*/
void set_run_time()
{
    srand((unsigned int)time(NULL));
    for (int i = 0; i < thread_number; i++)
        run_random[i] = rand() % 20 + 1;
}

/*
***
随机生成每个进程的优先级
***
*/
void set_priority()
{
    srand((unsigned int)time(NULL));
    for (int i = 0; i < thread_number; i++)
        priority_random[i] = rand() % 5 + 1;
}

/*
***
按照到达时间排序每个进程
***
*/
void sort_arrive_time()
{
    int n = thread_number;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
        {
            if (PCBS[i].arrive_time > PCBS[j].arrive_time)
            {
                PCB pcb_temp;
                pcb_temp = PCBS[i];
                PCBS[i] = PCBS[j];
                PCBS[j] = pcb_temp;
            }
        }
}

/*
***
按照所需时间排序每个进程
***
*/
void sort_need_time()
{
    int n = thread_number;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
        {
            if (PCBS[i].need_time > PCBS[j].need_time)
            {
                PCB pcb_temp;
                pcb_temp = PCBS[i];
                PCBS[i] = PCBS[j];
                PCBS[j] = pcb_temp;
            }
        }
}

/*
***
按照优先级排序每个进程
***
*/
void sort_priority()
{
    int n = thread_number;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
        {
            if (PCBS[i].priority > PCBS[j].priority)
            {
                PCB pcb_temp;
                pcb_temp = PCBS[i];
                PCBS[i] = PCBS[j];
                PCBS[j] = pcb_temp;
            }
        }
}

/*
***
初始化每个线程对应的虚拟PCB
***
*/
void *initPCB()
{
    srand((unsigned int)time(NULL));
    PCBS[thread_count].t_id = thread_count + 1;
    PCBS[thread_count].arrive_time = 0;
    PCBS[thread_count].need_time = run_random[thread_count];
    PCBS[thread_count].t_state = 0;
    PCBS[thread_count].priority = priority_random[thread_count];
    PCBS[thread_count].start_time = 0;
    PCBS[thread_count].run_time = 0;
    PCBS[thread_count].end_time = 0;
    PCBS[thread_count].wait_time = 0;
    thread_count++;
}

/*
***
重置每个虚拟PCB运行时修改的属性
***
*/
void resetPCB()
{
    for (int i = 0; i < thread_number; i++)
    {
        PCBS[thread_count].t_state = 0;
        PCBS[thread_count].start_time = 0;
        PCBS[thread_count].run_time = 0;
        PCBS[thread_count].end_time = 0;
        PCBS[thread_count].wait_time = 0;
    }
}

/*
***
FCFS调度算法
***
*/
void handleFCFS()
{
    sort_arrive_time();
    int run_time = 0;
    int flag = 0;
    int thread_index = 0;
    run_time = PCBS[thread_index].arrive_time;
    while (!flag)
    {
        if (PCBS[thread_index].run_time == 0)
            PCBS[thread_index].wait_time = run_time;
        run_time++;
        printf("Thread%d:%d\n", PCBS[thread_index].t_id, run_time);
        if (PCBS[thread_index].run_time == 0)
        {
            PCBS[thread_index].start_time = run_time - 1;
        }
        PCBS[thread_index].run_time++;
        if (PCBS[thread_index].run_time == PCBS[thread_index].need_time)
        {
            PCBS[thread_index].end_time = run_time - 1;
            PCBS[thread_index].t_state = 1;
            thread_index++;
            if (thread_index == 20)
            {
                flag = 1;
            }
        }
    }
    printf("=================================================FCFS=================================================\n");
    for (int i = 0; i < thread_number; i++)
    {
        printf("thread number  arrive_time  need_time  priority  start_time  run_time  end_time  waiting_time\n");
        printf("%15d%13d%11d%10d%12d%10d%10d%14d\n", PCBS[i].t_id, PCBS[i].arrive_time, PCBS[i].need_time, PCBS[i].priority, PCBS[i].start_time, PCBS[i].run_time, PCBS[i].end_time, PCBS[i].wait_time);
    }
    resetPCB();
}

/*
***
SJF调度算法
***
*/
void handleSJF()
{
    sort_need_time();
    int run_time = 0;
    int flag = 0;
    int thread_index = 0;
    run_time = PCBS[thread_index].arrive_time;
    while (!flag)
    {
        if (PCBS[thread_index].run_time == 0)
            PCBS[thread_index].wait_time = run_time;
        run_time++;
        printf("Thread%d:%d\n", PCBS[thread_index].t_id, run_time);
        if (PCBS[thread_index].run_time == 0)
        {
            PCBS[thread_index].start_time = run_time - 1;
        }
        PCBS[thread_index].run_time++;
        if (PCBS[thread_index].run_time == PCBS[thread_index].need_time)
        {
            PCBS[thread_index].end_time = run_time - 1;
            PCBS[thread_index].t_state = 1;
            thread_index++;
            if (thread_index == 20)
            {
                flag = 1;
            }
        }
    }
    printf("=================================================SJF=================================================\n");
    for (int i = 0; i < thread_number; i++)
    {
        printf("thread number  arrive_time  need_time  priority  start_time  run_time  end_time  waiting_time\n");
        printf("%15d%13d%11d%10d%12d%10d%10d%14d\n", PCBS[i].t_id, PCBS[i].arrive_time, PCBS[i].need_time, PCBS[i].priority, PCBS[i].start_time, PCBS[i].run_time, PCBS[i].end_time, PCBS[i].wait_time);
    }
    resetPCB();
}

/*
***
优先级调度算法
***
*/
void handlePRIORITY()
{
    sort_priority();
    int run_time = 0;
    int flag = 0;
    int thread_index = 0;
    run_time = PCBS[thread_index].arrive_time;
    while (!flag)
    {
        if (PCBS[thread_index].run_time == 0)
            PCBS[thread_index].wait_time = run_time;
        run_time++;
        printf("Thread%d:%d\n", PCBS[thread_index].t_id, run_time);
        if (PCBS[thread_index].run_time == 0)
        {
            PCBS[thread_index].start_time = run_time - 1;
        }
        PCBS[thread_index].run_time++;
        if (PCBS[thread_index].run_time == PCBS[thread_index].need_time)
        {
            PCBS[thread_index].end_time = run_time - 1;
            PCBS[thread_index].t_state = 1;
            thread_index++;
            if (thread_index == 20)
            {
                flag = 1;
            }
        }
    }
    printf("=================================================PRIORITY=================================================\n");
    for (int i = 0; i < thread_number; i++)
    {
        printf("thread number  arrive_time  need_time  priority  start_time  run_time  end_time  waiting_time\n");
        printf("%15d%13d%11d%10d%12d%10d%10d%14d\n", PCBS[i].t_id, PCBS[i].arrive_time, PCBS[i].need_time, PCBS[i].priority, PCBS[i].start_time, PCBS[i].run_time, PCBS[i].end_time, PCBS[i].wait_time);
    }
    resetPCB();
}

/*
***
RR调度算法
***
*/
void handleRR()
{
    sort_arrive_time();
    int run_time = 0;
    int flag = 0;
    int thread_index = 0;
    int thread_ac = 0;
    run_time = PCBS[thread_index].arrive_time;
    printf("please enter the length of time slice\n");
    int time_slice;
    scanf("%d", &time_slice);
    while (!flag)
    {
        if (PCBS[thread_index].t_state == 0)
        {
            int remain_time = 0;
            remain_time = PCBS[thread_index].need_time - PCBS[thread_index].run_time;
            if (remain_time >= time_slice)
            {
                run_time += time_slice;
                printf("Thread%d:%d\n", PCBS[thread_index].t_id, run_time);
                if (PCBS[thread_index].run_time == 0)
                {
                    PCBS[thread_index].start_time = run_time - 1;
                }
                PCBS[thread_index].run_time += time_slice;
                if (PCBS[thread_index].run_time == PCBS[thread_index].need_time)
                {
                    PCBS[thread_index].end_time = run_time - 1;
                    PCBS[thread_index].t_state = 1;
                    thread_ac++;
                    PCBS[thread_index].wait_time = run_time - PCBS[thread_index].run_time;
                    if (thread_ac == 20)
                    {
                        flag = 1;
                    }
                }
            }
            else
            {
                run_time += remain_time;
                printf("Thread%d:%d\n", PCBS[thread_index].t_id, run_time);
                if (PCBS[thread_index].run_time == 0)
                {
                    PCBS[thread_index].start_time = run_time - 1;
                }
                PCBS[thread_index].run_time += remain_time;
                if (PCBS[thread_index].run_time == PCBS[thread_index].need_time)
                {
                    PCBS[thread_index].end_time = run_time - 1;
                    PCBS[thread_index].t_state = 1;
                    thread_ac++;
                    PCBS[thread_index].wait_time = run_time - PCBS[thread_index].run_time;
                    if (thread_ac == 20)
                    {
                        flag = 1;
                    }
                }
                run_time = run_time - remain_time + time_slice;
            }
        }
        thread_index++;
        if (thread_index > 19)
            thread_index -= 20;
    }
    printf("=================================================RR=================================================\n");
    for (int i = 0; i < thread_number; i++)
    {
        printf("thread number  arrive_time  need_time  priority  start_time  run_time  end_time  waiting_time\n");
        printf("%15d%13d%11d%10d%12d%10d%10d%14d\n", PCBS[i].t_id, PCBS[i].arrive_time, PCBS[i].need_time, PCBS[i].priority, PCBS[i].start_time, PCBS[i].run_time, PCBS[i].end_time, PCBS[i].wait_time);
    }
    resetPCB();
}

/*
***
该函数创建20个线程，并进入初始化虚拟PCB的函数
***
*/
void *childFunc()
{
    printf("now create 20 threads\n");
    set_run_time();
    set_priority();
    for (int i = 0; i < 20; i++)
    {
        printf("run...\n");
        int ret_temp;
        pthread_t tid_temp;
        ret_temp = pthread_create(&tid_temp, NULL, &initPCB, NULL);
        if (ret_temp == 0)
        {
            printf("creating no.%d thread...\n", i);
            sleep(1);
        }
        else
        {
            printf("failed to create the no.%d thread...\n", i);
        }
    }
}

int main()
{
    int ret_t;
    pthread_t tid1;
    ret_t = pthread_create(&tid1, NULL, &childFunc, NULL);
    if (ret_t == 0)
    {
        printf("creating child threads ...\n\n");
        sleep(20);
    }
    else
    {
        printf("failed to create the thread\n");
    }
    //handleFCFS();
    //handleSJF();
    //handlePRIORITY();
    handleRR();
    return 0;
}