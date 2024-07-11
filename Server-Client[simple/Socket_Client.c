#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 100

int main()
{
    char buffer[BUFFER_SIZE];
    struct sockaddr_un client_address;
    int client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        perror("Can't create socket......");
        exit(EXIT_FAILURE);
    }
    client_address.sun_family = AF_UNIX;
    strcpy(client_address.sun_path, "/tmp/localSocket");
    int ret = connect(client_socket, (struct sockaddr *)&client_address, sizeof(client_address));
    if (ret < 0)
    {
        perror("Can't connect socket......");
        exit(EXIT_FAILURE);
    }
    printf("Enter the key you have ==> ");
    scanf("%s", buffer);
    write(client_socket, buffer, strlen(buffer) + 1);
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer));
    if (bytes_read < 0)
    {
        perror("Error reading from socket......");
        exit(EXIT_FAILURE);
    }
    buffer[bytes_read] = '\0';
    printf("The value ==> %s\n", buffer);
    return 0;
}
