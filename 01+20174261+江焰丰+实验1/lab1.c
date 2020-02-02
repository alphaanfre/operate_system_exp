#include<unistd.h>
#include<pthread.h>
#include<stdio.h>
#include<math.h>
#include<sys/types.h>
#include<sys/wait.h>

#define max_len 500

//primer number
void *MyThread1(void)
{
    int number;
    printf("now is the thread 1\n");
    printf("please enter a number bigger than 1\n");
    scanf("%d", &number);
    int count = 1;
    printf("1 ");
    for(int  i=2;i<=number;i++)
    {
        int flag = 0;
        for(int j=2;j<number/2;j++)
        {
            if(i % j == 0 && i != j)
            {
                flag = 1;
                break;
            }
        }
        if(flag == 0)
        {
            count++;
            printf("%d ", i);
            if(count % 5 == 0)
                printf("\n");
        }
    }
    printf("\n");
}

//Fibonacci  sequence
void *MyThread2(void)
{ 
    int len;
    printf("now is the thread 2\n");
    printf("please enter the sequence len bigger than 2\n");
    scanf("%d", &len);
    int  fib[max_len];
    fib[0] = 1;
    fib[1] = 1;
    printf("1 1 ");
    for(int i=2;i<len;i++)
    {
        fib[i] = fib[i-1] + fib[i-2];
        printf("%d ", fib[i]);
        if(i % 5 == 0)
            printf("\n");
    }
    printf("\n");
}

int thread_two()
{
    pthread_t  id1, id2;
    int err1, err2;
    err1 = pthread_create(&id1, NULL, MyThread1, NULL);
    if(err1 != 0)
    {
        printf("thread 1 can't create\n");
        return 0;
    }
    pthread_join(id1, NULL);

    err2 = pthread_create(&id2, NULL, MyThread2, NULL);
    if(err2 != 0)
    {
        printf("thread 2 can't create\n");
        return 0;
    }
    pthread_join(id2, NULL);
    printf("main thread exit!\n");
    return 1;
}

void Createprocess_two()
{
    int i ;
    pid_t p5, p6;
    p5 = fork();
    if(p5 == 0)
    {
        funct(5);
        return 0;
    }
    wait(NULL);
    p6 = fork();
    if(p6 == 0)
    {
        funct(6);
        return 0;
    }
    int st1, st2;
    waitpid(p5, &st1, 0);
    waitpid(p6, &st2, 0);
}

void funct(int i)
{
    int flag = 0;

    switch (i)
    {
    case 5:
        {
            printf("now is process no.5\n");
            flag = thread_two();// create the two thread
            if(flag == 0)
                printf("Error in process 5\n");
            flag = 0;
            break;
        }
    case 6:
        {
            printf("now is process no.6\n");
            system("./hello");
            break;
        }
    default:
        break;
    }
}

void childfunc(int i)
{
    int flag = 0;

    switch(i)
    {
        //system call ls, ps and free
        case 3:
            {
                printf("now is process 3\n");
                printf("now is ls command\n");
                system("ls");
                printf("now is ps command\n");
                system("ps");
                printf("now is free command\n");
                system("free");
                break;
            }
        //no.4 process create the no.5 process and no.6 process
        case 4: 
            {
                printf("now is process 4\n");
                Createprocess_two();
                break;
            }
        default:
            break;
    }
}

//no.1 process create the no.3 process and no.4 process
int Createprocess()
{
    int i ;
    int status;
 

    pid_t p3, p4;
    p3 = fork();
    if(p3 == 0)
    {
        childfunc(3);
        return 0;
    }
    wait(NULL);
    p4 = fork();
    if(p4 == 0)
    {
        childfunc(4);
        return 0;
    }
    int st1, st2;
    waitpid(p3, &st1, 0); //no.1 parent's process waits the child  process no.3
    waitpid(p4, &st2, 0);//no.1 parent's process waits the child  process no.4
    return 0;
}

int main()
{
    Createprocess();
    return 0;
}
