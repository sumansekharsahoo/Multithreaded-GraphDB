#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>
// Define the message format
typedef struct Request_t{
    int sequence_number;
    int operation_number;
    char graph_file[100]; // Num nodes @ Filename
    // For write operations
    // Add other fields as needed
}Request;

// Message Queue key and ID
#define MSG_KEY 1234
int msgid;

int main() {
    Request request;
    
    key_t keyq;
    //Creating message queue
    if ((keyq = ftok("load_balancer.c", 1000)) == -1)
    {
        perror("ftok");
        exit(1);
    }
    
    msgid = msgget(keyq, 0666 | IPC_CREAT);
    if (msgid == -1) {
        fprintf(stderr,"msgget");
        exit(1);
    }
    printf("Load Balancer is Listening: ");
    fflush(stdout);

    while (1) {
        if (msgrcv(msgid, &request, sizeof(request),0, 0) == -1) {
            fprintf(stderr,"msgrcv");
            exit(1);
        }
        
        if(request.operation_number == 1 && request.sequence_number <= 100){
            printf("\n%d",request.sequence_number);
            printf("\n%s",request.graph_file);
            fflush(stdout);

            request.sequence_number+=100;
            strcpy(request.graph_file,"Victory !");
            printf("%s",request.graph_file);
            printf("\nmtype modified to %d ",request.sequence_number);
            fflush(stdout);

            // Send the message to the message queue.
            if(msgsnd(msgid,&request,sizeof(request),0)==-1){
                        fprintf(stderr,"\nError in msgsnd");
                        exit(1);
                    }
        }
    }
    
    return 0;
}
