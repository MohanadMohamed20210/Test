#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <errno.h>

#define buffer_size (1024 * 4)  // 4k

typedef struct sockaddr_un local_socket_addr_t;
typedef struct sockaddr socket_addr_t;

int main()
{
    int num1, num2, result;
    char operator;
    local_socket_addr_t client_addr;
    char buffer[buffer_size];
    int client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    client_addr.sun_family = AF_UNIX;
    strcpy(client_addr.sun_path, "/tmp/localSocket");
    int ret = connect(client_socket, (socket_addr_t *)&client_addr, sizeof(local_socket_addr_t));
    if (ret < 0)
    {
        perror("server is down.....");
        exit(EXIT_FAILURE);
    }

    printf("Enter the operator (+, -, *, /): ");
    scanf(" %c", &operator);
    ret = write(client_socket, &operator, sizeof(char));
    if (ret < 0)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }

    printf("Enter the two numbers: ");
    scanf("%d", &num1);
    scanf("%d", &num2);
    ret = write(client_socket, &num1, sizeof(int));
    if (ret < 0)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }
    ret = write(client_socket, &num2, sizeof(int));
    if (ret < 0)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }

    ret = read(client_socket, &result, sizeof(int));
    if (ret < 0)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }
    printf("The response: %d %c %d = %d\n", num1, operator, num2, result);
    close(client_socket);
    return 0;
}