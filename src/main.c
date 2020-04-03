#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define CAL_ROOT "/srv/calserv"


int main()
{
  //set up some buffers and pointers
  char buffer[64];
  char command[16];
  char month[5] = "/";
  char day[4] = "/";
  char filepath[256];
  char *cmdptr; //used with strtok to extract commands from client
  FILE *fp;

  //Get a socket and set it up

  int client_socket;
  client_socket = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_address;
  server_address.sin_family = INADDR_ANY;
  server_address.sin_port = htons(32001);
  server_address.sin_addr.s_addr = INADDR_ANY;

  bind(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));

  listen(client_socket, 5);

  client_socket = accept(client_socket, NULL, NULL);
  printf("New connection.\n");

  while (1)
  {
    //recieve data and split up the command and its arguments
    recv(client_socket, buffer, sizeof(buffer), 0);
    cmdptr = strtok(buffer, " ");
    strcpy(command, cmdptr);

    if (strcmp(command, "READ") == 0)
    {
      cmdptr = strtok(NULL, " ");
      strcat(month, cmdptr);

      if (strcmp(month, "spe") == 0)
      {
        //put special case here. List whole year?
      }
      else
      {
        cmdptr = strtok(NULL, " \n");
        strcat(day, cmdptr);
        strcpy(filepath, month);
        strcat(filepath, day);
        printf("Requesting to read %s\n", filepath);  

      }

    }

  }

  close(client_socket);
  return 0;
}
