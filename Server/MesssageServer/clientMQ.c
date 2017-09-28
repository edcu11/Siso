#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define NAME_SERVER "/server"
#define GET_MESSAGES "read\n"
#define SEND_MESSAGE "send\n"
#define CLEAN_MESSAGES "limpiar\n"
#define LOGOUT "logout\n"
#define MESSAGE_SIZE 1024
#define ERR -1

int main(int argc, char ** argv){
  mqd_t mq_server, mq_client;
  mode_t mode_server, mode_client;
  pid_t pid;
  char * name_client, *username;
  char * tmp_user;
  int flags_server, flags_client;
  struct mq_attr attr;

  attr.mq_maxmsg = 50;              //Arbitrary
  attr.mq_msgsize = MESSAGE_SIZE;   //Arbitrary
  attr.mq_curmsgs = 0;              //No messages yet
  attr.mq_flags = 0;                //Nonblock so it waits on read or 0
  flags_server = O_WRONLY;          //Read
  flags_client = O_RDONLY | O_CREAT;          //Write
  mode_server = S_IWUSR;            //User Read
  mode_client = S_IRUSR;            //User Write

  mq_server = mq_open(NAME_SERVER, flags_server);
  if(mq_server == (mqd_t) ERR){
      printf("Error con el servidor\n");
      return ERR;
  }/* Opens the server message queue */

  name_client = malloc(sizeof(pid_t));
  pid = getpid();
  sprintf(name_client, "/%d", pid);
  mq_client = mq_open(name_client, flags_client, mode_server, &attr);
  if(mq_client == (mqd_t) ERR){
      printf("Error con el cliente\n");
      return ERR;
  }/* Opens the client message queue */

  if(mq_setattr(mq_server, &attr, NULL) == ERR){
      printf("Error con atributos \n");
      return ERR;
  }/* Sets attributes to the message queue */

  char * in_buffer;
  char * user_input;
  char out_buffer[MESSAGE_SIZE];

  user_input = malloc(MESSAGE_SIZE);
  in_buffer = malloc(MESSAGE_SIZE);
  printf("Inicio Sesion: ");
  fgets(user_input, MESSAGE_SIZE, stdin);
  username = malloc(strlen(user_input)-1);
  strncpy(username, user_input, strlen(user_input)-1);
  snprintf(out_buffer, MESSAGE_SIZE, "%d|%s|%s|", pid, "0", username);
  //printf("MessageQ: %s\n", out_buffer);
  if (mq_send (mq_server, out_buffer, strlen (out_buffer), 0) == -1) {
      printf("Error al crear usuario\n");
      return -1;
  }
  free(user_input);
  user_input = malloc(MESSAGE_SIZE);
  printf("MessageQ# " );

  while (fgets(user_input, MESSAGE_SIZE, stdin)){
      if(!strcmp(user_input, GET_MESSAGES)){
        //  printf("Fetching Messages...\n");
          snprintf(out_buffer, MESSAGE_SIZE, "%d|%d|", pid, 1);
        //  printf("Sending: %s\n", out_buffer);
          if (mq_send (mq_server, out_buffer, strlen (out_buffer), 0) == -1) {
              printf("Error al enviar mensaje\n");
              continue;
          }
          if(mq_receive(mq_client, in_buffer, MESSAGE_SIZE, NULL) == ERR){
              printf("Error al recibir mensaje\n");
              break;
          }
          printf("Mensajes:\n%s\n", in_buffer);
          in_buffer = malloc(MESSAGE_SIZE);
      }else if(!strcmp(user_input, SEND_MESSAGE)){
          printf("MessageQ#write to: ");
          fgets(user_input, MESSAGE_SIZE, stdin);
          tmp_user = calloc(1, strlen(user_input)-1);
          memcpy(tmp_user, user_input, strlen(user_input)-1);
          printf("MessageQ#Mensaje: ");
          fgets(user_input, MESSAGE_SIZE, stdin);
          snprintf(out_buffer, MESSAGE_SIZE, "%d|%d|%s|%s|", pid, 2, user_input, tmp_user);
          printf("MessageQ#Sending: %s\n", out_buffer);
          if (mq_send (mq_server, out_buffer, strlen (out_buffer), 0) == -1) {
              printf("Couldn't send message to server.\n");
              continue;
          }
          free(tmp_user);
      }
      }else if(!strcmp(user_input, LOGOUT)){
        if(mq_unlink(name_client) == ERR){
            printf("Error!\n");
            return ERR;
        }/* Unlinks the queue */
        exit(1);
      }
      free(user_input);
      user_input = malloc(MESSAGE_SIZE);
      printf("MessageQ# " );
  }
}
