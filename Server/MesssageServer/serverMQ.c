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
        printf("Error de server\n");
        return ERR;
    }/* Opens the message queue */

    if(mq_setattr(mqd, &attr, NULL) == ERR){
        printf("Error atributosn");
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
    for (;;){
        if(mq_receive(mqd, in_buffer, MESSAGE_SIZE, NULL) == ERR){
            printf("Mensaje no reconocido\n");
            break;
        }

        printf("Message received. ");
        strncpy(token, strtok(in_buffer, "|"), MESSAGE_SIZE);
        printf("PID: %s - ", token);
        tmp_pid = malloc(strlen(token));
        strncpy(tmp_pid, token, sizeof(tmp_pid));
        strncpy(token, strtok(NULL, "|"), MESSAGE_SIZE);
        printf("Code: %s", token);
        if(!strcmp(token, "0")){
          strncpy(token, strtok(NULL, "|\n"), MESSAGE_SIZE);
          printf(" - Username: %s\n", token);
          int x = find_user_number_by_name(token, users, user_amount);
          if(x < 0){
            users[user_amount].name = malloc(strlen(token) * sizeof(char));
            strncpy(users[user_amount].name, token, strlen(token));
            users[user_amount].pid = malloc(strlen(tmp_pid) * sizeof(char));
            strncpy(users[user_amount].pid, tmp_pid, strlen(tmp_pid));
            printf("Usuario creado: %s\n", users[user_amount].name, users[user_amount].pid);
            user_amount++;
          }else{
            users[x].pid = malloc(strlen(tmp_pid) * sizeof(char));
            strncpy(users[x].pid, tmp_pid, strlen(tmp_pid));
            printf("%s is back.\n", token);
          }
        }else if(!strcmp(token, "1")){
          printf("\n %s\n", tmp_pid);
          name_client = malloc(strlen(tmp_pid) + 1);
          sprintf(name_client, "/%s", tmp_pid);
          mq_client = mq_open(name_client, O_WRONLY, S_IWUSR, &attr);
          if(mq_client == (mqd_t) ERR){
              printf("haiduheofiubsdioub\n");
              return ERR;
          }/* Opens the message queue */
          out_buffer = malloc(MESSAGE_SIZE);
          struct user tmp = find_user(tmp_pid, users);
          snprintf(out_buffer, MESSAGE_SIZE, "%s", get_messages(tmp.message_amount, tmp.messages));
          printf("Sending: %s\n", out_buffer);
          if (mq_send (mq_client, out_buffer, strlen (out_buffer), 0) == -1) {
              printf("Couldn't send message to client.\n");
              continue;
          }
          free(out_buffer);
        }else if(!strcmp(token, "2")){
          printf("\n");
          strncpy(token, strtok(NULL, "|\n"), MESSAGE_SIZE);
          tmp_message = malloc(MESSAGE_SIZE);
          strncpy(tmp_message, token, strlen(token));
          token = strtok(NULL, "|");
          tmp_user = malloc(strlen(token));
          strncpy(tmp_user, token, strlen(token));
          for(int x = 0; x < user_amount; x++){
            printf("Comparing %s %d to %s %d...\n", users[x].name, (int)strlen(users[x].name), token, (int)strlen(token));
            if(!strcmp(users[x].name, tmp_user)){
              printf("Found User!\n");
              char * tmp_from = find_user(tmp_pid, users).name;
              users[x].messages[users[x].message_amount].from = malloc(strlen(tmp_from));
              strncpy(users[x].messages[users[x].message_amount].from, tmp_from, strlen(tmp_from));
              users[x].messages[users[x].message_amount].message = malloc(strlen(tmp_message));
              strncpy(users[x].messages[users[x].message_amount].message, tmp_message, strlen(tmp_message));
              printf("User %s received a new message from %s!\n", tmp_user, users[x].messages[users[x].message_amount].from);
              users[x].message_amount++;
            }
          }
          free(tmp_message);
          free(tmp_user);
        }else if(!strcmp(token, "3")){
          int tmp = find_user_number(tmp_pid, users);
          users[tmp].message_amount = 0;
          free(users[tmp].messages);
          users[tmp].messages = calloc(10, sizeof(struct message));
          printf("\nCleaned %s's inbox!\n", users[tmp].name);
        }
        free(in_buffer);
        in_buffer = malloc(MESSAGE_SIZE);
        token = malloc(MESSAGE_SIZE);
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
