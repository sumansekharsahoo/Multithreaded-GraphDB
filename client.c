#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>
#include<sys/shm.h>
#define MAX_GRAPH_SIZE 256

// Define the message format
typedef struct Request_t {
    int sequence_number;
    int operation_number;
    char graph_file[100]; //Num nodes @ FileName
     // For write operations
    // Add other fields as needed
}Request;



// Message Queue key and ID
#define MSG_KEY 1234
int msgid;
// Shared Memory key and ID
#define SHM_KEY 1234
int shmid;





int main() {
    Request request;
    
    key_t keyq,keysm;
    
    char diff='@';
    
    //Creating message queue
    if ((keyq = ftok("load_balancer.c", 1000)) == -1)
    {
        perror("ftok");
        exit(1);
    }

    // Create or get the message queue
    msgid = msgget(keyq, 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }
    
    if ((keysm = ftok("load_balancer.c", request.sequence_number)) == -1)
    {
        perror("ftok");
        exit(1);
    }


    // Create or get the shared memory
    shmid = shmget(keysm, MAX_GRAPH_SIZE * MAX_GRAPH_SIZE * sizeof(int), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory to the process
    int *shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (int *)-1) {
        perror("shmat");
        exit(1);
    }

    // Input sequence number
    printf("Enter Sequence Number: ");
    scanf("%d", &request.sequence_number);
   

    // Input operation number
    printf("Enter Operation Number: ");
    scanf("%d", &request.operation_number);

    // Input graph file name
    printf("Enter Graph File Name: ");
    scanf("%s", request.graph_file);


    if (request.operation_number == 1) {
        // For write operations (option 1)
        int num_nodes;
        char temp[100];
        printf("Enter number of nodes of the graph: ");
        scanf("%d",&num_nodes);
        
        // code block which changes the graph_file accordingly
        sprintf(temp, "%d", num_nodes);  
        strncat(temp,&diff,1);
        strcat(temp,request.graph_file);
        strcpy(request.graph_file,temp);
        
       

        // Store the adjacency matrix data in the shared memory segment.
        for (int i = 0; i < num_nodes; i++) {
            for (int j = 0; j < num_nodes; j++) {
                printf("Enter adjacency matrix element at row %d, column %d: ", i, j);
                scanf("%d", &shared_memory[i * num_nodes + j]);
            }
        }

    }
     else if (request.operation_number == 2) {
        // For other write operations, add or delete nodes/edges
        // Additional logic to input and validate the changes
    }
     else if (request.operation_number == 3 || request.operation_number == 4) {
        // For read operations (options 3 and 4)
        // Input and store the starting vertex
        printf("Enter starting vertex: ");
        int starting_vertex;
        scanf("%d", &starting_vertex);
        // Store the starting vertex in the shared memory
        shared_memory[0] = starting_vertex;
    }

    // Send the request to the load balancer
        if (msgsnd(msgid, &request, sizeof(Request), 0) == -1) {
        fprintf(stderr,"msgsnd");
        exit(1);
        }

    // Recive from the sever ...

        printf("hi");
        if(msgrcv(msgid,&request,sizeof(request),request.sequence_number+100,0)==-1){
            fprintf(stderr,"\nError in msgrcv");
            exit(1);
        }
        printf("%s",request.graph_file);
        fflush(stdout);
        request.sequence_number -=100;
    // Detach the shared memory from the process
    shmdt(shared_memory);

    return 0;
}
