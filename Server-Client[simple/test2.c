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
#define socket_port 80
typedef struct sockaddr_in local_socket_addr_t;
typedef struct sockaddr socket_addr_t;

int client_socket;

void handle_sigint(int sig) 
{
    printf("Caught signal %d\n", sig);
    if (client_socket >= 0) {
        close(client_socket);
        printf("Socket closed\n");
    }
    exit(0);
}

void make_request(const char *request, char *response_buffer) 
{
    local_socket_addr_t client_addr;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) 
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(socket_port);
    inet_pton(AF_INET, "35.173.69.207", &(client_addr.sin_addr));
    int ret = connect(client_socket, (socket_addr_t *)&client_addr, sizeof(local_socket_addr_t));
    if (ret < 0) 
    {
        perror("server is down.....");
        exit(EXIT_FAILURE);
    }

    ret = write(client_socket, request, strlen(request));
    if (ret < 0) 
    {
        perror("write");
        exit(EXIT_FAILURE);
    }
    
    ret = read(client_socket, response_buffer, buffer_size - 1);
    if (ret < 0) 
    {
        perror("read");
        exit(EXIT_FAILURE);
    }
    
    response_buffer[ret] = '\0';
    close(client_socket);
    client_socket = -1;
}

void WriteIntoFile(const char *str) 
{
    FILE *file = fopen("info.txt", "w");
    if (file == NULL) 
    {
        perror("Error opening file");
        return;
    }
    fprintf(file, "%s", str);
    fflush(file);
    fclose(file);
}

int main() 
{
    signal(SIGINT, handle_sigint);

    char buffer[buffer_size];
    char buffer2[buffer_size];

    const char *request1 = "GET /pins HTTP/1.1\r\n"
                           "Host: mohanad012.pythonanywhere.com\r\n"
                           "Connection: close\r\n"
                           "User-Agent: CustomClient/1.0\r\n"
                           "Accept: */*\r\n"
                           "\r\n";
    
    make_request(request1, buffer);

    int i = 0, j = 0;
    char temp[buffer_size];
    int flag = 0;
    while (buffer[i] != '\0') {
        if ((buffer[i - 1] == '>') || (flag == 1)) {
            flag = 1;
            temp[j] = buffer[i];
            j++;
        }
        i++;
    }
    temp[j] = '\0';
    WriteIntoFile(temp);

    const char *request2 = "GET /sys HTTP/1.1\r\n"
                           "Host: mohanad012.pythonanywhere.com\r\n"
                           "Connection: close\r\n"
                           "User-Agent: CustomClient/1.0\r\n"
                           "Accept: */*\r\n"
                           "\r\n";
    
    while (1) 
    {
        make_request(request2, buffer2);
        i = 0;
        j = 0;
        char temp2[buffer_size];
        while (buffer2[i] != '\0') 
        {
            if (buffer2[i] == '>') 
            {
                temp2[j] = buffer2[i + 1];
                j++;
            }
            i++;
        }
        temp2[j] = '\0';
        printf("%s\n", temp2);
        sleep(1);
    }
    
    return 0;
}
