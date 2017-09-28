#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SERVER_NAME "/server"
#define MESSAGE_SIZE 1024
#define ERR -1

struct message {
  char * from;
  char * message;
};

struct user {
  char * name;
  char * pid;
  int message_amount;
  struct message * messages;
};
struct user find_user(char * pid_str, struct user users[10]);
int find_user_number(char * pid_str, struct user users[10]);
int find_user_number_by_name(char * name, struct user users[10], int max);
char * get_messages(int message_amount, struct message messages[10]);

int main(int argc, char** argv) {
    mqd_t mqd, mq_client;
    mode_t mode;
    int flags;
    int user_amount = 0;
    char * tmp_pid, * tmp_message, *tmp_user, *name_client;
    struct mq_attr attr;
    struct user users[10];

    attr.mq_maxmsg = 50;                //Arbitrary
    attr.mq_msgsize = MESSAGE_SIZE;     //Arbitrary
    attr.mq_curmsgs = 0;                //No messages yet
    attr.mq_flags = 0;                  //Nonblock so it waits on read or 0
    flags = O_RDONLY | O_CREAT;         //Read/Write & Create
    mode = S_IRUSR | S_IWUSR;           //User Read & Write

    mqd = mq_open(SERVER_NAME, flags, mode, &attr);
    if(mqd == (mqd_t) ERR){
        printf("Error Opening - Server exited with -1\n");
        return ERR;
    }/* Opens the message queue */

    if(mq_setattr(mqd, &attr, NULL) == ERR){
        printf("Error Setting Attributes - Server exited with -1\n");
        return ERR;
    }/* Sets attributes to the message queue */

    char * in_buffer;
    char * out_buffer;
    char * token;

    for(int x = 0; x < 10; x++){
      users[x].message_amount = 0;
      users[x].messages = malloc(sizeof(struct message)*10);
    }

    in_buffer = malloc(MESSAGE_SIZE);
    token = malloc(MESSAGE_SIZE);
    for (;;)
    {

        if(mq_receive(mqd, in_buffer, MESSAGE_SIZE, NULL) == ERR)
        {
            printf("Error Receiving Message\n");
            break;
        }
        else
          printf("Message received. ");


    }/* Polls for messages */

    if(mq_unlink(SERVER_NAME) == ERR){
        printf("Error Unlinking - Server exited with -1\n");
        return ERR;
    }/* Unlinks the queue */

    printf("Server exited with 0\n");
    return 0;
}

struct user find_user(char * pid_str, struct user users[10]){
  for(int x = 0; x < 10; x++){
    if(!strcmp(pid_str, users[x].pid)){
      return users[x];
    }
  }
}

int find_user_number(char * pid_str, struct user users[10]){
  for(int x = 0; x < 10; x++){
    if(!strcmp(pid_str, users[x].pid)){
      return x;
    }
  }
}

int find_user_number_by_name(char * name, struct user users[10], int max){
  for(int x = 0; x < max; x++){
    if(!strcmp(name, users[x].name)){
      return x;
    }
  }
  return -1;
}

char * get_messages(int message_amount, struct message messages[10]){
  char * returnValue = malloc(MESSAGE_SIZE);
  for(int x = 0; x < message_amount; x++){
    char * buffer = malloc(MESSAGE_SIZE);
    sprintf(buffer, "\nFrom: %s\nMessage: %s\n", messages[x].from, messages[x].message);
    if(x == 0){
      strcpy(returnValue, buffer);
    }else{
      strcat(returnValue, buffer);
    }
  }
  return returnValue;
}
