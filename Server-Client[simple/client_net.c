#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define BUFFER_SIZE (1024 * 10)  // 10k
#define SOCKET_PORT 443

int main()
{
    // Initialize SSL
    SSL_library_init();
    SSL_CTX *ssl_ctx = SSL_CTX_new(SSLv23_client_method());
    if (!ssl_ctx)
    {
        perror("SSL_CTX_new");
        exit(EXIT_FAILURE);
    }

    // Create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Resolve server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SOCKET_PORT);
    struct hostent *server = gethostbyname("mohanadmohamed.pythonanywhere.com");
    if (!server)
    {
        perror("gethostbyname");
        exit(EXIT_FAILURE);
    }
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // Set up SSL connection
    SSL *ssl = SSL_new(ssl_ctx);
    if (!ssl)
    {
        perror("SSL_new");
        exit(EXIT_FAILURE);
    }
    SSL_set_fd(ssl, client_socket);
    if (SSL_connect(ssl) <= 0)
    {
        perror("SSL_connect");
        exit(EXIT_FAILURE);
    }

    // Construct HTTP GET request
    char get_request[] = "GET /state.txt HTTP/1.1\r\n"
                         "Host: mohanadmohamed.pythonanywhere.com\r\n"
                         "Connection: close\r\n"
                         "\r\n";
    printf("%ld\n",strlen(get_request));

    // Send request
    if (SSL_write(ssl, get_request, strlen(get_request)) <= 0)
    {
        perror("SSL_write");
        exit(EXIT_FAILURE);
    }

    // Receive response
    char buffer[BUFFER_SIZE];
    int bytes_received;
    while ((bytes_received = SSL_read(ssl, buffer, BUFFER_SIZE)) > 0)
    {
        fwrite(buffer, 1, bytes_received, stdout);
    }
    if (bytes_received < 0)
    {
        perror("SSL_read");
        exit(EXIT_FAILURE);
    }

    // Clean up
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ssl_ctx);
    close(client_socket);

    return 0;
}