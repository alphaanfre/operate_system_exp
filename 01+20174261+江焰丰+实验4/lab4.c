#include<stdio.h>
#include<string.h>

#define PAGESIZE 1000
#define PROCESSNUM 6
#define SAMPLINGNUM 100
#define PAGEREQNUM 2000000

int pageFrame[PROCESSNUM][PAGESIZE];
int pageReq[PROCESSNUM][PAGEREQNUM];
double pageFault[PROCESSNUM][SAMPLINGNUM];
int frameNow[PROCESSNUM];

void FIFO(int pId, int reqTime)
{
    int faultNum = 0;
    int recordTime = 0;
    int faultEveryRecord = 0;
    for(int i=0;i<reqTime;i++)
    {
        int tempFrame = pageReq[pId][i];
        int flag = 0;
        for(int j=0;j<frameNow[pId];j++)
        {
            if(tempFrame==pageFrame[pId][j])
            {
                flag = 1;
                break;
            }
        }
        if(flag == 0)
        {
            faultNum++;
            faultEveryRecord++;
            //printf("page no.%d fault!!!\n", pageReq[pId][i]);
            //printf("now fault number is %d\n", faultNum);
            if(frameNow[pId] < PAGESIZE)
            {
                pageFrame[pId][frameNow[pId]] = tempFrame;
                frameNow[pId] += 1;
            }
            else
            {
                int index = faultNum % 1000;
                pageFrame[pId][index] = tempFrame;
            }
        }
        if(i%10000==9999)
        {
            pageFault[pId][recordTime++] = faultEveryRecord / (10000 * 1.0);
            faultEveryRecord = 0;
        }
    }
    //for(int i=1;i<=recordTime;i++)
    //	printf("after request time is %d, the page fault is %.2lf\n", i*10000,pageFault[pId][i-1]);
    printf("the process no.%d, the page fault is %.2lf\n", pId+1, faultNum / (reqTime*1.0));
}

void LRU(int pId, int reqTime)
{
    int faultNum = 0;
    int recordTime = 0;
    int faultEveryRecord = 0;
    int frameCount[PAGESIZE];
    int frameTime[PAGESIZE];
    memset(frameCount, 0, sizeof(frameCount));
    memset(frameTime, 0, sizeof(frameTime));
    for(int i=0;i<reqTime;i++)
    {
        int foundIndex = -1;
        int tempFrame = pageReq[pId][i];
        int flag = 0;
        for(int j=0;j<frameNow[pId];j++)
        {
            if(tempFrame==pageFrame[pId][j])
            {
                flag = 1;
                foundIndex = j;
                break;
            }
        }
        if(flag==1)
        {
            //frameCount[foundIndex] += 1;
            frameTime[foundIndex] = i;
        }
        if(flag==0)
        {
            faultNum++;
            faultEveryRecord++;
            //printf("page no.%d fault!!!\n", pageReq[pId][i]);
            //printf("now fault number is %d\n", faultNum);
            if(frameNow[pId] < PAGESIZE)
            {
                pageFrame[pId][frameNow[pId]] = tempFrame;
                //frameCount[frameNow[pId]] = 1;
                frameTime[frameNow[pId]] = i;
                frameNow[pId] += 1;
            }
            else
            {
                int index;
                //int minCount = 10000000;
                int minIndex = 0;
                int minTime = 10000000;
                // for(int k=0;k<frameNow[pId];k++)
                // {
                //     if(frameCount[k] < minCount)
                //     {
                //         minCount = frameCount[k];
                //         minTime = frameTime[k];
                //         minIndex = k;
                //     }
                //     else if(frameCount[k] == minCount)
                //     {
                //         if(frameTime[k] < minTime)
                //         {
                //             minTime = frameTime[k];
                //             minIndex = k;
                //         }
                //     }
                // }
                for(int k=0;k<frameNow[pId];k++)
                {
                    if(frameTime[k] < minTime)
                    {
                        minTime = frameTime[k];
                        minIndex = k;
                    }
                }
                index = minIndex;
                pageFrame[pId][index] = tempFrame;
                //frameCount[index] = 1;
                frameTime[index] = i;                
            }
        }
        if(i%100000==99999)
        {
            pageFault[pId][recordTime++] = faultEveryRecord / (100000 * 1.0);
            faultEveryRecord = 0;
        }
    }
    //for(int i=1;i<=recordTime;i++)
    //	printf("after request time is %d, the page fault is %.2lf\n", i*100000,pageFault[pId][i-1]);
    printf("the process no.%d, the page fault is %.2lf\n", pId+1, faultNum / (reqTime*1.0));
}

void CLOCK(int pId, int reqTime)
{
    int faultNum = 0;
    int recordTime = 0;
    int faultEveryRecord = 0;
    int frameTag[PAGESIZE];
    memset(frameTag, 0, sizeof(frameTag));
    int tagIndex = 0;
    for(int i=0;i<reqTime;i++)
    {
        int tempFrame = pageReq[pId][i];
        int flag = 0;
        int foundIndex = -1;
        for(int j=0;j<frameNow[pId];j++)
        {
            if(tempFrame==pageFrame[pId][j])
            {
                flag = 1;
                foundIndex = j;
                break;
            }
        }
        if(flag==1)
        {
            frameTag[foundIndex] = 1;
        }
        if(flag==0)
        {
            faultNum++;
            faultEveryRecord++;
            //printf("page no.%d fault!!!\n", pageReq[pId][i]);
            //printf("now fault number is %d\n", faultNum);
            if(frameNow[pId] < PAGESIZE)
            {
                pageFrame[pId][frameNow[pId]] = tempFrame;
                frameTag[frameNow[pId]] = 1;
                frameNow[pId] += 1;
            }
            else
            {
                int changeFlag = 0;
                while(!changeFlag)
                {
                    for(int k=tagIndex;k<frameNow[pId];k++)
                    {
                        if(frameTag[k]==1)
                        {
                            frameTag[k] = 0;
                        }
                        else
                        {
                            pageFrame[pId][k] = tempFrame;
                            frameTag[k] = 1;
                            changeFlag = 1;
                            tagIndex = (k + 1) % frameNow[pId];
                            break;
                        }
                    }
                    if(changeFlag == 0)
                        tagIndex = 0;
                }
            }
        }
        if(i%100000==99999)
        {
            pageFault[pId][recordTime++] = faultEveryRecord / (100000 * 1.0);
            faultEveryRecord = 0;
        }
    }
    //for(int i=1;i<=recordTime;i++)
    //	printf("after request time is %d, the page fault is %.2lf\n", i*100000,pageFault[pId][i-1]);
    printf("the process no.%d, the page fault is %.2lf\n", pId+1, faultNum / (reqTime*1.0));    
}

void loadFile()
{
    FILE *fp;
    char* fileName[15] = {"workload1","workload2","workload3","workload4","workload5","workload6"};
    printf("now is the CLOCK algorithm\n");
    for(int i=0;i<6;i++)
    {
    	printf("Now is process no.%d\n", i+1);
        if((fp = fopen(fileName[i], "r")) == NULL)
        {
            printf("error in open file\n");
        }
        int n;
        int count = 0;
        while(fscanf(fp, "%d", &n))
        {
            //printf("%d\n", n);
            pageReq[i][count++] = n;
            if(feof(fp))
                break;
        }
        fclose(fp);
        //FIFO(i, count);
        //LRU(i, count);
        CLOCK(i, count);        
    }
}

int main()
{
    loadFile();
    return 0;  
}