#include<stdio.h>
#include<unistd.h>
#include<sys/msg.h>
#include<string.h>
#include<sys/types.h>
#include<stdlib.h>

typedef struct  the_message{ 
    int message_type;
    char text[128];
}msg;

int getqid()
{
    key_t key = ftok(".", 't');
    int msqid = msgget(key, 0666 | IPC_CREAT);
    return msqid;
}
void check_send(int write_flag)
{
    if(write_flag == -1)
        printf("failed to send message\n");
    return;
}

void message_com()
{
    int msqid = getqid();
    pid_t pid1, pid2;
    size_t temp_size;
    pid1 = fork();
    if(pid1 < 0)
    {
        printf("failed to create the child process 1\n");
    }
    else if(pid1 == 0)
    {
        FILE *fp;
        fp = fopen("1.txt", "w");
        if (fp == NULL)
        {
            printf("failed to create file\n");
            return;
        }
        else
        {
            int flag = 12;
            fprintf(fp, "%d", flag);
        }
        fclose(fp);
        msg msg1;
        msg1.message_type = 1;
        strcpy(msg1.text, "OK");
        int write_flag;
        temp_size = sizeof(msg1);
        write_flag = msgsnd(msqid, &msg1, sizeof(msg1), IPC_NOWAIT);
        check_send(write_flag);
        printf("the process 1 sent the message to the queue\n");
        exit(0);
    }
    wait(NULL);
    pid2 = fork();
    if (pid2 < 0)
    {
        printf("failed to create the child process 2\n");
    }
    else if(pid2 == 0)
    {
        msg msg2;
        int read_flag;
        read_flag = msgrcv(msqid, &msg2, temp_size, 0, IPC_NOWAIT);
        //nbytes = read(r_fd, readbuffer, sizeof(readbuffer));
        if (read_flag != 0)
        {
            printf("the process 2 receive the message from the queue\n");
            FILE *fp;
            fp = fopen("1.txt", "r");
            if (fp == NULL)
            {
                printf("failed to open the file\n");
                return;
            }
            else
            {
                int flag;
                fscanf(fp, "%d", &flag);
                printf("the flag is %d\n", flag);
            }
            fclose(fp);
        }
        exit(0);
    }
}

int main()
{
    printf("now is the message commnuication\n");
    message_com();
    return 0;
}