#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define CAL_ROOT "/Users/CharlieSchild/calserv"

//adds character "ch" to string "str"
void addchar(char *str, char ch);


int main()
{
  //set up some buffers and pointers
  char ch;
  char buffer[161]; //Includes space for null
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

  //start main loop. first level checks for commands
  while (1)
  {
    //recieve data and split up the command and its arguments
    recv(client_socket, buffer, sizeof(buffer), 0);
    cmdptr = strtok(buffer, " ");
    strcpy(command, cmdptr);

    if (strcmp(command, "READ") == 0)//Simply display reminder/s
    {
      cmdptr = strtok(NULL, " ");
      strcat(month, cmdptr);

      if (strcmp(month, "/spe") == 0)
      {
        //put special case here. List whole year?
      }

      else
      {
        cmdptr = strtok(NULL, " \n");
        strcat(day, cmdptr);

        if (strcmp(day, "/0") == 0)
        {
          //List whole month
        }

        else
        {
          strcpy(filepath, CAL_ROOT);
          strcat(filepath, month);
          strcat(filepath, day);
          printf("Requesting to read %s\n", filepath);
          strcpy(buffer, "");
          //opens the filepath from the command
          fp = fopen(filepath, "r");
          if ((ch = fgetc(fp)) == '!')//handles important notices
          {
            strcpy(buffer, "Important: ");
            ch = fgetc(fp);
          }

          while (ch != EOF)//read file and sends each notice
          {
            if (ch == '\n')//indicates the end of a notice
            {
              addchar(buffer, '/0');
              send(client_socket, buffer, strlen(buffer), 0);
              strcpy(buffer, "");
              ch = fgetc(fp);
              if (ch == EOF)
              {
                fseek(fp, -1, SEEK_CUR);
              }
            }
            addchar(buffer, ch);
            ch = fgetc(fp);
          }
          if (strlen(buffer) > 1)
          {
            addchar(buffer, '/0');
            
            send(client_socket, buffer, strlen(buffer), 0);
          }
        }

      }

    }

  }

  close(client_socket);
  return 0;
}

//adds character "ch" to string "str"
void addchar(char *str, char ch)
{
  int i = strlen(str);
  str[i] = ch;
  str[i + 1] = '\0';
}
