#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define N 5

int state[5];
#define STATE_THINKING 11
#define STATE_HUNGRY 12
#define STATE_EATING 13
pthread_mutex_t chopstick[N], mutex_e;
sem_t max_people;

int checkstate(int pos)
{
    return state[pos];
}

void think(int pos)
{
    printf("第%d号哲学家在思考\n\n", pos + 1);
    state[pos] = STATE_THINKING;
    state[pos] = STATE_HUNGRY;
    sleep(3);
}

void eat(int pos)
{
    printf("第%d号哲学家在吃饭\n\n", pos + 1);
    state[pos] = STATE_EATING;
    state[pos] = STATE_THINKING;
    sleep(3);
}

void takeforks(int pos, int flag)
{
    int left = pos % N;
    int right = (pos + 1) % N;
    if (flag == 0)
    {
        pthread_mutex_lock(&chopstick[left]);
        pthread_mutex_lock(&chopstick[right]);
    }
    else if (flag == 1)
    {
        if ((pos + 1) % 2 == 0)
        {
            pthread_mutex_lock(&chopstick[right]); //方法二，如果为偶数哲学家，则先尝试拿右筷子
            pthread_mutex_lock(&chopstick[left]);
        }
        else
        {
            pthread_mutex_lock(&chopstick[left]);
            pthread_mutex_lock(&chopstick[right]); //方法二，如果为奇数哲学家，则先尝试拿左筷子
        }
    }
}

void putforks(int pos, int flag)
{
    int left = pos % N;
    int right = (pos + 1) % N;
    if (flag == 0)
    {
        pthread_mutex_unlock(&chopstick[left]);
        pthread_mutex_unlock(&chopstick[right]);
    }
    else if (flag == 1)
    {
        if ((pos + 1) % 2 == 0)
        {
            pthread_mutex_unlock(&chopstick[right]);
            pthread_mutex_unlock(&chopstick[left]);
        }
        else
        {
            pthread_mutex_unlock(&chopstick[left]);
            pthread_mutex_unlock(&chopstick[right]);
        }
    }
}

/*
***
方法一
***
*/
void *creatPhilosopherMethod1(void *index)
{
    int pos = (int)index;
    printf("你好，我是第%d号哲学家\n\n", pos + 1);
    while (1)
    {
        think(pos);
        if (checkstate(pos) == STATE_HUNGRY)
        {
            pthread_mutex_lock(&mutex_e);
            takeforks(pos, 0);
            eat(pos);
            putforks(pos, 0);
            pthread_mutex_unlock(&mutex_e);
        }
    }
}

/*
***
方法二
***
*/
void *creatPhilosopherMethod2(void *index)
{
    int pos = (int)index;
    printf("你好，我是第%d号哲学家\n\n", pos + 1);
    while (1)
    {
        think(pos);
        if (checkstate(pos) == STATE_HUNGRY)
        {
            pthread_mutex_lock(&mutex_e);
            takeforks(pos, 1);
            eat(pos);
            putforks(pos, 1);
            pthread_mutex_unlock(&mutex_e);
        }
    }
}

/*
***
方法三
***
*/
void *creatPhilosopherMethod3(void *index)
{
    int pos = (int)index;
    printf("你好，我是第%d号哲学家\n\n", pos + 1);
    while (1)
    {
        think(pos);
        if (checkstate(pos) == STATE_HUNGRY)
        {
            sem_wait(&max_people); //每次最多四个哲学家同时拿起左筷子
            takeforks(pos, 1);
            eat(pos);
            putforks(pos, 1);
            sem_post(&max_people);
        }
    }
}

int main()
{
    pthread_t t_id[N];
    int ret[N];
    int method;
    printf("请输入你想使用哪一种方法\n");
    scanf("%d", &method); //选择想要的方法，0代表方法一，1代表方法二，2代表方法三

    /*
    ***
    0代表选择方法一(仅当哲学家左右两个筷子都拿起，才能进餐)。
    1代表选择方法二（奇数号哲学家先拿左手，偶数好哲学家先拿右手）。
    2代表选择方法三（一次最多只能有四个哲学家拿起左筷子）。
    ***
    */

    for (int i = 0; i < N; i++)
    {
        pthread_mutex_init(&chopstick[i], NULL); //初始化5个筷子的信号量
    }
    if (method != 2)
        pthread_mutex_init(&mutex_e, NULL); //如果不为方法三，创建一个每个哲学家的互斥量
    else if (method == 2)
        sem_init(&max_people, 0, 4); //如果为方法三，创建一个一次最多4个哲学家同时拿起左筷子的信号量
    for (int i = 0; i < N; i++)
    {
        //判断选择的方法，进入不同的函数
        if (method != 1)
            ret[i] = pthread_create(&t_id[i], NULL, &creatPhilosopherMethod1, (void *)i);
        else if (method == 1)
            ret[i] = pthread_create(&t_id[i], NULL, &creatPhilosopherMethod2, (void *)i);
        else if (method == 2)
            ret[i] = pthread_create(&t_id[i], NULL, &creatPhilosopherMethod3, (void *)i);
        if (ret[i] != 0)
        {
            printf("第%d号哲学家创建成功...\n\n", i + 1);
        }
        else
        {
            printf("第%d号哲学家创建成功...\n\n", i + 1);
        }
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(t_id[i], NULL);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_mutex_destroy(&chopstick[i]);
    }

    if (method != 2)
        pthread_mutex_destroy(&mutex_e);
    return 0;
}