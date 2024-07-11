#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>

#define buffer_size (1024 * 10)
#define socket_port      80
typedef struct sockaddr_in local_socket_addr_t;
typedef struct sockaddr socket_addr_t;


int main()
{
    local_socket_addr_t client_addr;
    char buffer[buffer_size];
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    client_addr.sin_family = AF_INET;
    client_addr.sin_port=htons(socket_port);
    inet_pton(AF_INET,"35.173.69.207",&(client_addr.sin_addr));
    int ret = connect(client_socket, (socket_addr_t *)&client_addr, sizeof(local_socket_addr_t));
    if (ret < 0)
    {
        perror("server is down.....");
        exit(EXIT_FAILURE);
    }
    sprintf(buffer, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", "/files", "mohanad012.pythonanywhere.com");
    
    ret = write(client_socket, buffer, sizeof(buffer));
    if (ret < 0)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }
    ret = read(client_socket, buffer , sizeof(buffer));
    if (ret < 0)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }
    char Freq[20];
    int j=0;
    int flag = 0;
    for (int i = 0; buffer[i]!='\0' ; i++)
    {
        if((buffer[i] == '>')||(flag == 1))
        {
            flag = 1;
            Freq[j]=buffer[i+1];
            j++;
        }
    }
    printf("%s\n",Freq);
    close(client_socket);

    return 0;
}