#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


void RearrangeUser(char * tempUser, char * temp)
{
  tempUser = calloc(1, strlen(temp)-1);
  memcpy(tempUser, temp, strlen(temp)-1);
}

void CreateMessage(char buffer[], int pid, char * temp, char * tempUser)
{
  snprintf(buffer, 128, "%d|%d|%s|%s|", pid, 2, temp, tempUser);
}

int OpenServer(int flags_server, mqd_t mq_server)
{
  mq_server = mq_open("/server", flags_server);
  if(mq_server == (mqd_t) -1){
      printf("Error abriendo servidor! \n");
      return -1;
  }/* Opens the server message queue */

}

int CheckErrors(mqd_t mq_client,  mqd_t mq_server, struct mq_attr attr)
{
  if(mq_client == (mqd_t) -1){
      printf("Error abriendo cliente! \n");
      return -1;
  }/* Opens the client message queue */

  if(mq_setattr(mq_server, &attr, NULL) == -1){
      printf("Error de atrinutos! \n");
      return -1;
  }/* Sets attributes to the message queue */

}

int main(int argc, char ** argv){
  mqd_t mq_server;
  mqd_t mq_client;
  mode_t mode_server;
  mode_t mode_client;
  pid_t pid;
  char * name_client;
  char * username;
  int flags_server;
  int flags_client;
  struct mq_attr attr;


  attr.mq_maxmsg = 50;              //Arbitrary
  attr.mq_maxmsg = 50;              //Arbitrary
  attr.mq_msgsize = 128;   //Arbitrary
  attr.mq_curmsgs = 0;              //No messages yet
  attr.mq_flags = 0;                //Nonblock so it waits on read or 0
  flags_server = O_WRONLY;          //Read
  flags_client = O_RDONLY | O_CREAT;          //Write
  mode_server = S_IWUSR;            //User Read
  mode_client = S_IRUSR;            //User Write

  mq_server = mq_open("/server", flags_server);
  if(mq_server == (mqd_t) -1){
      printf("Error Opening Server MQ - Client exited with -1\n");
      return -1;
  }/* Opens the server message queue */

  name_client = malloc(sizeof(pid_t));
  pid = getpid();
  sprintf(name_client, "/%d", pid);
  mq_client = mq_open(name_client, flags_client, mode_server, &attr);
  if(mq_client == (mqd_t) -1){
      printf("Error Opening Client MQ - Client exited with -1\n");
      return -1;
  }/* Opens the client message queue */

  if(mq_setattr(mq_server, &attr, NULL) == -1){
      printf("Error Setting Attributes - Server exited with -1\n");
      return -1;
  }/* Sets attributes to the message queue */


  char * in_buffer;
  char * temp;
  char * tempUser;
  char out_buffer[128];

  temp = malloc(128);
  in_buffer = malloc(128);

  printf("Login or create: ");
  fgets(temp, 128, stdin);
  username = malloc(strlen(temp)-1);
  strncpy(username, temp, strlen(temp)-1);

  snprintf(out_buffer, 128, "%d|%s|%s|", pid, "0", username);
  printf("Sending: %s\n", out_buffer);
  printf("haodsi\n");

  if (mq_send (mq_server, out_buffer, strlen (out_buffer), 0) == -1) {
      printf("Error de usuario!\n");
      return -1;
  }
  free(temp);
  temp = malloc(128);
  while(1)
  {
    printf("Messenger#" );
    fgets(temp, 128, stdin);

    if(strcmp(temp, "send") == 0)
    {
      printf("Write to: ");
      fgets(temp, 128, stdin);

      RearrangeUser(tempUser, temp);

      printf("message: ");
      fgets(temp, 128, stdin);
      CreateMessage(out_buffer, pid, temp, tempUser);
      if (mq_send (mq_server, out_buffer, strlen (out_buffer), 0) == -1) {
          printf("Couldn't send message to server.\n");
          continue;
      }
      free(tempUser);


    }


  }




}
