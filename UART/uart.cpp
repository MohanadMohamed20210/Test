#include <iostream>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>

#define BUFFER_SIZE (1024 * 10)
#define SOCKET_PORT 80

typedef struct sockaddr_in local_socket_addr_t;
typedef struct sockaddr socket_addr_t;

int client_socket;
int serial_port;

void signal_handler(int sig) 
{
    serialClose(serial_port);
    close(client_socket);
    printf("Socket closed\n");
    exit(0);
}
char *make_request(const char *request) {
    local_socket_addr_t client_addr;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(SOCKET_PORT);
    inet_pton(AF_INET, "35.173.69.207", &(client_addr.sin_addr));
    int ret = connect(client_socket, (socket_addr_t *)&client_addr, sizeof(local_socket_addr_t));
    if (ret < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    ret = write(client_socket, request, strlen(request));
    if (ret < 0) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    char *response_buffer = (char *)malloc(BUFFER_SIZE);
    if (response_buffer == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    ret = read(client_socket, response_buffer, BUFFER_SIZE - 1);
    if (ret < 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    response_buffer[ret] = '\0';

    close(client_socket);
    client_socket = -1;

    return response_buffer;
}

int main() 
{
    signal(SIGINT, signal_handler);
    char data_buffer[BUFFER_SIZE];

    if ((serial_port = serialOpen("/dev/serial0", 115200)) < 0) {
        std::cerr << "Unable to open serial device!" << std::endl;
        return 1;
    }

    wiringPiSetup();
    if (wiringPiSetup() == -1) {
        std::cerr << "Unable to start wiringPi!" << std::endl;
        return 1;
    }

    if (serialDataAvail(serial_port)) {
        char digit = serialGetchar(serial_port);
        if (digit == 'h' || digit == 'l' || digit == 'v' || digit == 'm') 
        {
            data_buffer[0] = digit;
            char request2[BUFFER_SIZE * 2];
            snprintf(request2, BUFFER_SIZE * 2,
            "POST /sys HTTP/1.1\r\n"
            "Host: mohanad012.pythonanywhere.com\r\n"
            "Connection: close\r\n"
            "User-Agent: CustomClient/1.0\r\n"
            "Accept: */*\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Content-Length: %zu\r\n"
            "\r\n"
            "data=%s",
            strlen(data_buffer) + 5, data_buffer); 

            printf("Sending Request:\n%s\n", request2);
            char *response = make_request(request2);
            printf("Server Response:\n%s\n", response);
            free(response);

        }
    }

    serialClose(serial_port);
    close(client_socket);

    return 0;
}
