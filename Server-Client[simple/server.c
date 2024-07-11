#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <errno.h>

#define buffer_size (1024 * 4)  // 4k

typedef struct sockaddr_un local_socket_addr_t;
typedef struct sockaddr socket_addr_t;

int server_socket;

void destroy_signal(int sig)
{
    printf("Destroying allocated server_socket.....\n");
    close(server_socket);
    unlink("/tmp/localSocket");
}

int main()
{
    int num1, num2, result;
    char operator;
    signal(SIGTERM, destroy_signal);
    char buffer[buffer_size];
    local_socket_addr_t server_addr;
    unlink("/tmp/localSocket");
    int server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    printf("server socket created\n");

    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, "/tmp/localSocket", strlen("/tmp/localSocket") + 1);
    int ret = bind(server_socket, (socket_addr_t *)&server_addr, sizeof(server_addr));
    if (ret < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    ret = listen(server_socket, 2);
    if (ret < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        ret = read(client_socket, &operator, sizeof(char));
        if (ret < 0)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }

        ret = read(client_socket, &num1, sizeof(int));
        if (ret < 0)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }

        ret = read(client_socket, &num2, sizeof(int));
        if (ret < 0)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }

        switch (operator)
        {
            case '+':
            {
                result = num1 + num2;
                break;
            }
            case '-':
            {
                result = num1 - num2;
                break;
            }
            case '*':
            {
                result = num1 * num2;
                break;
            }
            case '/':
            {
                result = num1 / num2;
                break;
            }
            default:
            {
                strcpy(buffer, "wrong operator");
                ret = write(client_socket, buffer,sizeof(buffer));
                if (ret < 0)
                {
                    perror("write");
                    exit(EXIT_FAILURE);
                }
            }
        }

        ret = write(client_socket, &result, sizeof(int));
        if (ret < 0)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
        close(client_socket);
    }
    return 0;
}