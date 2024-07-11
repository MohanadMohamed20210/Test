#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <signal.h>

#define TRUE 1
#define FALSE 0
#define POSSIBLE_CONNECTIONS 2
#define BUFFER_SIZE 100

char* getValue(char *buffer);
int server_socket;
void destroy_signal(int sig)
{
    printf("Destroying allocated server_socket.....\n");
    close(server_socket);
    unlink("/tmp/localSocket");
}

int main()
{
    char buffer[BUFFER_SIZE];
    struct sockaddr_un server_address;
    signal(SIGTERM, destroy_signal);
    server_socket = socket(AF_UNIX,SOCK_STREAM,FALSE);
    if(server_socket < FALSE)
    {
        perror("Can't create socket......");
        exit(EXIT_FAILURE);
    }
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path,"/tmp/localSocket");
    int ret = bind(server_socket,(struct sockaddr *)&server_address,sizeof(server_address));
    if(ret < FALSE)
    {
        perror("Can't bind socket......");
        exit(EXIT_FAILURE);
    }
    ret = listen(server_socket,POSSIBLE_CONNECTIONS);
    if(ret < FALSE)
    {
        perror("Can't asgin listen devices......");
        exit(EXIT_FAILURE);
    }
    while(TRUE)
    {
        int client_socket = accept(server_socket,NULL,NULL);
        if(client_socket < FALSE)
        {
            perror("Can't get client socket......");
            exit(EXIT_FAILURE);
        }
        if((read(client_socket,buffer,sizeof(buffer))) > FALSE)
        {
            printf("The recieved key ==> %s\n",buffer);
            char *value = getValue(buffer);
            if(value != NULL)
            {
                write(client_socket,value,strlen(value));
            }
        }
        close(client_socket);
    }
    close(server_socket);
    return 0;
}

char* getValue(char *buffer)
{
    int flag = FALSE;
    int i = 0;
    static char value[20];
    FILE *file = fopen("file.json", "r");
    if (file == NULL)
    {
        perror("Can't open file......");
        return NULL;
    }
    char json[100];
    fgets(json, sizeof(json), file);
    printf("The file content ==> %s\n",json);
    fclose(file);
    int iterator_i = 0, iterator_j = 1;
    while (json[iterator_i] != '\0')
    {
        if (json[iterator_i] == ':')
        {
            i = 0;
            char temp[20];
            while (iterator_j != iterator_i)
            {
                temp[i] = json[iterator_j];
                i++;
                iterator_j++;
            }
            temp[i] = '\0';
            printf("The parsed ==> %s\n",temp);
            if (strcmp(temp, buffer) == 0)
            {
                flag = TRUE;
                i = 0;
                int k = iterator_i;
                k++;
                while (json[k] != ',' && json[k] != '\0')
                {
                    value[i] = json[k];
                    i++;
                    k++;
                }
                value[i] = '\0';
                return value;
            }
        }
        if (json[iterator_i] == ',')
        {
            iterator_j = iterator_i + 1;
        }
        iterator_i++;
    }
    if (flag == FALSE)
    {
        printf("Key is not exist.......");
        return NULL;
    }
}
