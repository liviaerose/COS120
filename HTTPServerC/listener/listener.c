
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <time.h>

#define MYHOST "localhost"
//#define MYPORT "8080"
#define MAX_CLIENT_BACKLOG 128
#define MAX_BUFFER_SIZE 4096
#define IS_MULTIPROCESS 1


void send_response(int accept_desc, char * path){
    char response_buffer[MAX_BUFFER_SIZE];
    memset(response_buffer, 0, MAX_BUFFER_SIZE);
    unsigned long i;
    int bytes_sent;
    time_t current_time = time(0);
    struct tm tm = *localtime(&current_time);
    strftime(response_buffer, sizeof (response_buffer), "%a, %d %b %Y %H:%M:%S %Z\r\n", &tm);
    send(accept_desc, "Date: ", 6,0);
    send(accept_desc, response_buffer, 31,0);

    send(accept_desc, "Content-Type: ", 14 ,0);
    const char period = '.';

    char * type = (strrchr(path, period));

    if(type == NULL){
        send(accept_desc, "Unknown Type\r\n", 14, 0);
    }
    else if(strcmp(type,".jpg") == 0){
        send(accept_desc, "image/jpeg\r\n", 12, 0);
    }
    else if (strcmp(type,".png") == 0) {
        send(accept_desc, "image/png\r\n", 11, 0);
    }
    else if (strcmp(type,".gif") == 0) {
        send(accept_desc, "image/gif\r\n", 11, 0);
    }
    else if (strcmp(type,".pdf") == 0) {
        send(accept_desc, "application/pdf\r\n", 17, 0);
    }
    else if (strcmp(type,".js") == 0) {
        send(accept_desc, "application/javascript\r\n", 24, 0);
    }
    else if (strcmp(type,".html") == 0) {
        send(accept_desc, "text/html\r\n", 11, 0);
    }
    else if (strcmp(type,".txt") == 0) {
        send(accept_desc, "text/txt\r\n", 10, 0 );
    }
    else if (strcmp(type,".css") == 0) {
        send(accept_desc, "text/css\r\n", 10 , 0);
    }
    else{
        send(accept_desc, type, strlen(type), 0);
        send(accept_desc, "\r\n", 2, 0);
    }
}

void handle_connection(int accept_desc, char * docroot) {
    char c;
    int bytes_read;
    int cursor = 0;
    char request_buffer[MAX_BUFFER_SIZE];

    memset(request_buffer, 0, MAX_BUFFER_SIZE);

    long recv_bytes = recv(accept_desc, &request_buffer, MAX_BUFFER_SIZE, 0);
    printf("%d\r\n", (int)recv_bytes);

    char* HTTP_request = strtok(request_buffer, "\n");
    char* method = strtok(HTTP_request, " ");
    char* path = strtok(NULL, " ");
    for (int i = 0; i < (int) strlen(path); i++) {
        path[i] = path[i+1];
    }
    char *subset = strtok(NULL, " ");
    strncat(docroot, "/", 15);
    strncat(docroot, path, 15);
    if (strncmp("GET", method, 3) == 0 || strncmp("HEAD", method, 4) == 0) {
        if (strncmp("HTTP/1.1", subset, 8) == 0 || strncmp("HTTP/1.0", subset, 8) == 0) {
            FILE * desc = fopen(docroot, "r");
            if (desc >=-1 || desc==NULL) {
                if(strncmp("HTTP/1.1", subset, 8) == 0 ){
                    send(accept_desc, "HTTP/1.1 404 NOT FOUND\r\n", 24, 0);
                }else {
                    send(accept_desc, "HTTP/1.0 404 NOT FOUND\r\n", 24, 0);
                }
                send_response(accept_desc, path);
            } else {
                if(strncmp("HTTP/1.1", subset, 8) == 0 ){
                    send(accept_desc, "HTTP/1.1 200 OK\r\n",17, 0 );
                }else {
                    send(accept_desc, "HTTP/1.0 200 OK\r\n", 17 ,0);
                }
                send_response(accept_desc, path);
                send(accept_desc, "Content-Length: ", 16, 0);
                fseek(desc, 0L, SEEK_END);
                int size = ftell(desc);
                dprintf(accept_desc, "%d\r\n", (int) size);
                send(accept_desc, "\r\n", 2, 0);
                rewind(desc);
                char readout[size];
                int readin;
                while ((readin = fread(readout, size, 1, desc) > 0)) {
                    send(accept_desc, readout, size, 0);
                }
                fclose(desc);
            }
        } else {
            if(strncmp("HTTP/1.1", subset, 8) == 0 ){
                send(accept_desc, "HTTP/1.1 400 BAD REQUEST\r\n", 26, 0);
            }else {
                send(accept_desc, "HTTP/1.0 400 BAD REQUEST\r\n", 26, 0);
            }
            send_response(accept_desc, path);
        }
    }
}


int main(int argc, char* argv[]) {

    int socket_desc, accept_desc;
    int return_value;
    int enable = 1;
    char * port;
    char * docroot;

    if(argc >= 3) {
        for (short i = 1; i < argc; ++i) {
            //printf("\nargv %d: %s", i, argv[i]);
            if (i == 2) {
                docroot = argv[2];
            }else if (i == 3) {
                int check = atoi(argv[3]);
                if (check > 65535) {
                    printf("\nError, please enter valid port number.\n", check);
                } else if (check < 0) {
                    printf("\nError, please enter valid port number %d.\n", check);
                } else if (check < 1024) {
                    printf("\nWarning, port number %d is probably reserved.\n", check);
                }
                port = argv[3] ;
            }
        }
    }

    struct addrinfo hints;
    struct addrinfo *address_resource;
    struct sockaddr_storage remote_addr;
    socklen_t remote_addr_s = sizeof(remote_addr);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; //IPv4
    hints.ai_socktype = SOCK_STREAM; //TCP
    hints.ai_flags = AI_PASSIVE; //listen



    return_value = getaddrinfo(MYHOST, port, &hints, &address_resource);
    if (return_value != 0) {
        printf("Error: %s (line: %d)\n, strerror(errno), __LINE__");
        return return_value;
    }

    // socket(): creates a new TCP/IP socket, no address or port is assigned yet
    socket_desc = socket(
            address_resource->ai_family,
            address_resource->ai_socktype,
            address_resource->ai_protocol
    );
    if (socket_desc == -1) {
        printf("Error: %s (line: %d)\n, strerror(errno), __LINE__");
        return socket_desc;
    }

    return_value = setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    if (return_value == -1) {
        printf("Error: %s (line: %d)\n, strerror(errno), __LINE__");
        return return_value;
    }

    //bind(): assign address and port to socket
    return_value = bind(socket_desc, address_resource->ai_addr, address_resource->ai_addrlen);
    if (return_value == -1) {
        printf("Error: %s (line: %d)\n, strerror(errno), __LINE__");
        return return_value;
    }

    freeaddrinfo(address_resource);


    return_value = listen(socket_desc, MAX_CLIENT_BACKLOG);
    if (return_value == -1) {
        printf("Error: %s (line: %d)\n, strerror(errno), __LINE__");
        return return_value;
    }

    while (1) {
        accept_desc = accept(socket_desc, (struct sockaddr *) &remote_addr, &remote_addr_s);
#if IS_MULTIPROCESS == 1
        int pid = fork();
        //printf(pid);
        if(pid==0){
            //child process starts...

            handle_connection(accept_desc, docroot);
            close(accept_desc);
            close(socket_desc);
            exit(0);

            //...child process terminated
        }
        if(pid==-1){
            printf("Error: %s (line: %d)\n, strerror(errno), __LINE__");
            return pid;
        }
        //parent process
        close(accept_desc);

#else
        handle_connection(accept_desc, docroot);
            close(accept_desc);

#endif
    }
    close(socket_desc);
    return 0;
}