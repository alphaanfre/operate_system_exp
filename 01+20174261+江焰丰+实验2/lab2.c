#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>

void pipe_com()
{
    int fd[2], nbytes;
    char readbuffer[20];	
    pipe(fd);
    int r_fd = fd[0];
    int w_fd = fd[1];
    pid_t child1, child2;
    child1 = fork();
    if(child1 < 0)
    {
        printf("failed to create the child process 1\n");
    } 
    else if(child1 == 0)
    {
        FILE *fp;
        fp = fopen("1.txt", "w");
        if(fp == NULL)
        {
            printf("failed to create file\n");
            return;
        }
        else
        {
            int flag = 12;
            printf("now the child 1 process input the flag to the file\n");
            fprintf(fp, "%d", flag); 
        }
        close(r_fd);
        write(w_fd, "ok", 2);
        fclose(fp);
        exit(0);
    }
    wait(NULL);
    child2 = fork();
    if(child2 < 0 )
    {
        printf("failed to create the child process 2\n");
    }
    else 
    {
        nbytes = read(r_fd, readbuffer, sizeof(readbuffer));
        if (strcmp(readbuffer, "ok") == 0)
        {
            FILE  *fp;
            fp = fopen("1.txt", "r");
            if(fp == NULL)
            {
                printf("failed to open the file\n");
                return;
            }
            else
            {
                int flag;
                fscanf(fp, "%d", &flag);
                printf("now the child 2 process read the flag from the file\n");
                printf("the flag is %d\n", flag);
            }
            fclose(fp);
        }
        exit(0);
        return;
    }
    return;
}

int main()
{
    printf("now is the pipe communication\n");
    pipe_com();
    return 0;
}

