#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/shm.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_VERTICES 1000

int msgid;

typedef struct Stack_t
{

    char *vertices;
    char *ans;
    bool *visited;
    char **adjacencyMatrix;
    int ans_ptr;
    char *cur_vertex;
    char start_vertex;
    int num_vertices;
    int top;

} Stack;

typedef struct LBRequest_t
{
    long mtype;
    long sequence_number;
    int operation_number;
    char graph_file_name[100];
} LBRequest;

typedef struct Result_t
{
    long mtype;
    char mtext[100];
} Result;

void *bfs(void *argument)
{
    printf("BFS Function")
        LBRequest lbRequest = *((LBRequest *)argument);
    // printf("")
}

void *dfs(void *argument)
{
    printf("DFS Function")
        LBRequest lbRequest = *((LBRequest *)argument);
    // printf("")
}

int main()
{

    key_t keyq;
    pthread_t threads[100];
    int i = 0;

    if ((keyq = ftok("LoadBalancer.c", 1000)) == -1)
    {
        perror("ftok");
        exit(1);
    }

    // Create or get the message queue
    msgid = msgget(keyq, 0666);
    if (msgid == -1)
    {
        perror("msgget");
        exit(1);
    }

    int flag;
    printf("Enter Secondary Server number:\n1-> Start Secondary Server 1\n2-> Start Secondary Server 2");
    scanf("%d", &flag);
    flag--;

    while (true)
    {
        if (flag)
        {
            printf("Secondary Server 2 is Listening!!");
        }
        else
        {
            printf("Secondary Server 1 is Listening!!");
        }
        fflush(stdout);

        LBRequest lbRequest;

        if (msgrcv(msgid, &lbRequest, sizeof(lbRequest), 202 + flag, 0) == -1)
        {
            fprintf(stderr, "msgrcv");
            exit(1);
        }

        if (lbRequest.sequence_number == 101)
        {
            if (flag)
            {
                printf("Secondary Server 2 Terminating !!\n");
            }
            else
            {
                printf("Secondary Server 1 Terminating !!\n");
            }
            break;
        }

        if (lbRequest.operation_number == 3)
        {
            if (pthread_create(&threads[i], NULL, dfs, (void *)&lbRequest) != 0)
            {
                fprintf(stderr, "Error creating thread \n");
                exit(EXIT_FAILURE);
            }
            i++;
        }
        else if (lbRequest.operation_number == 4)
        {
            if (pthread_create(&threads[i], NULL, bfs, (void *)&lbRequest) != 0)
            {
                fprintf(stderr, "Error creating thread \n");
                exit(EXIT_FAILURE);
            }
            i++;
        }
    }

    // joining threads
    for (int j = 0; j < i; j++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            fprintf(stderr, "Error joining thread %d\n", i + 1);
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}