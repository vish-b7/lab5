#include "http-server.h"
#include <string.h>

int num = 0;

char const HTTP_404_NOT_FOUND[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";
char const HTTP_200_OK[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";

void handle_404(int client_sock, char *path)  {
    printf("SERVER LOG: Got request for unrecognized path \"%s\"\n", path);

    char response_buff[BUFFER_SIZE];
    snprintf(response_buff, BUFFER_SIZE, "Error 404:\r\nUnrecognized path \"%s\"\r\n", path);
    // snprintf includes a null-terminator
    // send resopnse back to client
    write(client_sock, HTTP_404_NOT_FOUND, strlen(HTTP_404_NOT_FOUND));
    write(client_sock, "Not Found", strlen(response_buff));
}


void handle_response(char *request, int client_sock) {
    char path[256];
    char response_buff[BUFFER_SIZE];

    printf("\nSERVER LOG: Got request: \"%s\"\n", request);

    // parse the path out of the request line (limit buffer size; sscanf null-terminates)
    if (sscanf(request, "GET %255s", path) != 1) {
        printf("Invalid request line\n");
        return;
    }
    else if (strcmp(path, "/shownum") == 0) {
        write(client_sock, HTTP_200_OK, strlen(HTTP_200_OK));
        snprintf(response_buff, BUFFER_SIZE, "Your number is %d", num);
        write(client_sock, response_buff, strlen(response_buff));
    }
    else if (strcmp(path, "/increment") == 0) {
        num++;
        write(client_sock, HTTP_200_OK, strlen(HTTP_200_OK));
        snprintf(response_buff, BUFFER_SIZE, "Incremented to %d", num);
        write(client_sock, response_buff, strlen(response_buff));
    }
    else if (strncmp(path, "/add?value=", 11) == 0) {
        int value;
        // parse the value out of the query string
        if (sscanf(path, "/add?value=%d", &value) == 1) {
            num += value;
            write(client_sock, HTTP_200_OK, strlen(HTTP_200_OK));
            snprintf(response_buff, BUFFER_SIZE, "Added %d to your number", value);
            write(client_sock, response_buff, strlen(response_buff));
        }
    }
    else {
        handle_404(client_sock, path);
    }
}

int main(int argc, char *argv[]) {
    int port = 0;
    if(argc >= 2) { // if called with a port number, use that
        port = atoi(argv[1]);
    }

    start_server(&handle_response, port);
}
