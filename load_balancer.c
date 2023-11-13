#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#define PERMS 0666

//message mtext of type: <Sequence_Number Operation_Number Graph_File_Name>
//Sequence number having 3 digits from 1 to 100 for the clients
//Operation number has one digit range from 1 to 4
typedef struct msgbuf_t
{
    long mtype;
    char mtext[200];
} msgbuf;

int main()
{
    msgbuf buf;
    int msgqid;
    int len;
    key_t key;
    //Creating message queue
    if ((key = ftok("load_balancer.c", 85)) == -1)
    {
        perror("ftok");
        exit(1);
    }
    if ((msgqid = msgget(key, PERMS | IPC_CREAT)) == -1)
    {
        perror("Failed to assign Message queue identifier");
        exit(1);
    }

    //serving the clients
    while(true){
        char opno='';
        //receiving the message from the message queue (client ids from 1 to 100)
        if (msgrcv(msgqid, &buf, sizeof(buf), -100, 0) == -1)
        {
            perror("msgrcv");
            exit(1);
        }
        //receiving the message from the cleanup process
        if(buf.mtype==501){
            //Using loop to indicate that the servers should terminate. mtype=502 for primary server, mtype=503 and mtype=504 for secondary servers 1 and 2 respectively
            for(int i=0;i<3;i++){
                buf.mtype++;
                if (msgsnd(msgqid, &buf, sizeof(buf), 0) == -1)
                        {
                            perror("Failed to send message");
                            exit(1);
                        }
            }
            //sleep for 5 seconds and then exit the loop
            sleep(5);
            break;
        }
    
        //Taking the opno as operation number from the received message
        opno=buf.mtext[3];
        switch(opno){
            //if opno is 1, we'll add 100 to client id and send the message back to the message queue, so that the message will be received and operations will be performed by primary server
            case '1':
                buf.mtype+=100;
                if (msgsnd(msgqid, &buf, sizeof(buf), 0) == -1)
                    {
                        perror("Failed to send message");
                        exit(1);
                    }
            //if opno is 2, we'll add 200 to client id and send the message back to the message queue, so that the message will be received and operations will be performed by primary server
            case '2':
                buf.mtype+=200;
                if (msgsnd(msgqid, &buf, sizeof(buf), 0) == -1)
                    {
                        perror("Failed to send message");
                        exit(1);
                    }
            //if opno is 3, we'll add 300 to client id and send the message back to the message queue, so that the message will be received and operations will be performed by secondary server 1
            case '3':
                buf.mtype+=300;
                if (msgsnd(msgqid, &buf, sizeof(buf), 0) == -1)
                    {
                        perror("Failed to send message");
                        exit(1);
                    }
            //if opno is 4, we'll add 400 to client id and send the message back to the message queue, so that the message will be received and operations will be performed by secondary server 2
            case '4':
                buf.mtype+=400;
                if (msgsnd(msgqid, &buf, sizeof(buf), 0) == -1)
                    {
                        perror("Failed to send message");
                        exit(1);
                    }
        }
    }

    //clearing and terminating the message queue
    if (msgctl(msgqid, IPC_RMID, NULL) == -1)
    {
        perror("msgctl");
        exit(1);
    }

    return 0;
}