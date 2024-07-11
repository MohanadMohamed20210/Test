#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#define BUFFER_SIZE 1024
#define SOCKET_PORT 80
#define SERVER_IP "35.173.69.207" // Replace with your server IP

int serial_port;

void handle_sigint(int sig) {
    printf("Caught signal %d\n", sig);
    serialClose(serial_port);
    exit(0);
}

void make_request(const char *data) {
    int client_socket;
    struct sockaddr_in server_addr;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Prepare server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SOCKET_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // Construct HTTP POST request
    char request[BUFFER_SIZE];
    snprintf(request, BUFFER_SIZE,
             "POST /sys HTTP/1.1\r\n"
             "Host: mohanad012.pythonanywhere.com\r\n"
             "Content-Type: application/x-www-form-urlencoded\r\n"
             "Content-Length: %zu\r\n"
             "\r\n"
             "data=%s",
             strlen(data), data);

    printf("Sending Request:\n%s\n", request);

    // Send request
    if (send(client_socket, request, strlen(request), 0) < 0) {
        perror("send");
        exit(EXIT_FAILURE);
    }

    // Close socket
    close(client_socket);
}

int main() {
    signal(SIGINT, handle_sigint);

    // Open serial port
    if ((serial_port = serialOpen("/dev/serial0", 115200)) < 0) {
        perror("serialOpen");
        exit(EXIT_FAILURE);
    }

    // Initialize wiringPi
    if (wiringPiSetup() == -1) {
        perror("wiringPiSetup");
        exit(EXIT_FAILURE);
    }

    char uart_data[BUFFER_SIZE];
    memset(uart_data, 0, BUFFER_SIZE);

    // Read data from UART
    int index = 0;
    while (1) {
        if (serialDataAvail(serial_port)) {
            char digit = serialGetchar(serial_port);
            if (digit == 'h' || digit == 'l' || digit == 'v' || digit == 'm') {
                printf("UART Data: %s\n", digit);
                    // Send HTTP POST request with UART data
                make_request(&digit);
            }
        }

    }

    



    // Close serial port
    serialClose(serial_port);

    return 0;
}
