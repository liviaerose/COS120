#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>

#define MAX_BUFFER_SIZE 1458
#define MAX_RETRIES 8

/*
 * I have been mulling over the code, and I keep getting lost in all of the details that we have to keep track of.
 * I think it will be easier if we write some pseudocode of what needs to be done before we do it. That way we have
 * something we can look at as a model in our code and go from there. So, here is some pseudocode that I have made
 * for server.c. This is not necessary, but I think a great place to start before we get too lost in our own code. I
 * know I feel that way, and maybe so do you. So, here it is:
 *
 * SEQ packet header:
 * 0  1  2  3  4  5  6  7  8  9 10 11    ...
 * | SEQ  STR | SEQ  NUM | DATA SIZE |   DATA   |
 * | 4  Bytes | 4  Bytes |  4 Bytes  | ?  Bytes |
 *
 * ACK packet header:
 * 0  1  2  3  4  5  6  7
 * | ACK  STR | ACK  NUM |
 * | 4  Bytes | 4  Bytes |
 *
 * ERR packet header:
 * 0  1  2  3  4  5  6  7  8  9 10 11    ...
 * | ERR  STR | NULL NUM | DATA SIZE | ERR  MSG |
 * | 4  Bytes | 4  Bytes |  4 Bytes  | ?  Bytes |
 *
 * send_acknowledgement():
 *      make ACK header with ACK STR and ACK NUM from SEQ num;
 *      send header to client;
 *
 * wait_for_acknowledgement():
 *      seq_num = char_to_int(data);
 *      while(i < MAX_RETRIES):
 *          wait for 2 seconds;
 *              recv ack;
 *          if no recv in 2 seconds:
 *              send data again;
 *          else:
 *              ack_num = char_to_data(recv);
 *              if (seq_num == ack_num):
 *                  break;
 *          i++;
 *      clear(recv);
 *
 * send_error_message():
 *      copy "ERR" onto buffer;
 *      copy error message to buffer;
 *      copy error size to buffer;
 *      send buffer to client;
 *      wait_for_acknowledgement();
 *
 * send_file():
 *      if you can open file:
 *          while not at EOF, read from file:
 *              if buffer is not full:
 *                  write data into buffer;
 *              else:
 *                  manage data header info
 *                  send data;
 *                  wait_for_acknowledgement();
 *          at end of while, at this point it has written to buff without sending it;
 *          send buff;
 *      else:
 *          return error; (file can't be opened == -2; doesn't exist == -1)
 *      close file;
 *
 * handle_connection():
 *      recv data;
 *      if data == SEQ:
 *          store SEQ num;
 *          send_acknowledgement();
 *          read file name;
 *          send_file();
 *          if it fails to open file:
 *              send ERR to client saying "file not found!";
 *      else:
 *          don't respond, or respond with an ERR saying "bad request" or something;
 *
 * main():
 *      open socket, listening for a connection;
 *      handle_connection();
 */


void int_to_char(unsigned char * data, unsigned int a) {
    // convert SEQ and ACK number from int to char[]
    data[0] = (a >> 24) & 0xFF;
    data[1] = (a >> 16) & 0xFF;
    data[2] = (a >> 8) & 0xFF;
    data[3] = a & 0xFF;
}

unsigned int char_to_int(const unsigned char * data) {
    // convert SEQ and ACK number from char[] to int
    unsigned int a = (unsigned int)(
            (unsigned char)(data[0]) << 24 |
            (unsigned char)(data[1]) << 16 |
            (unsigned char)(data[2]) << 8  |
            (unsigned char)(data[3]));
    return a;
}

/*
 * send_acknowledgement():
 *      make ACK header with ACK STR and ACK NUM from SEQ num;
 *      send header to client;
 */

int send_acknowledgement(int socket_desc, struct sockaddr_storage remote_addr, socklen_t addr_len, unsigned int ack_num) {
    int return_value;
    unsigned char * cursor;
    unsigned char data[8];
    memset(data, 0, sizeof(data));

    memcpy(data, "ACK", 4);
    cursor = &data[4];
    int_to_char(cursor, ack_num);

    return_value = (int)sendto(socket_desc, data, sizeof(data), 0, (struct sockaddr *) &remote_addr, addr_len);
    if (return_value == -1) {
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return return_value;
    }
    printf("server -> ACK %d -> client\n", ack_num);

    memset(data, 0, sizeof(data));
    return 0;
}

/*
 * wait_for_acknowledgement():
 *      seq_num = char_to_int(data);
 *      while(i < MAX_RETRIES):
 *          wait for 2 seconds;
 *              recv ack;
 *          if no recv in 2 seconds:
 *              send data again;
 *          else:
 *              ack_num = char_to_data(recv);
 *              if (seq_num == ack_num):
 *                  break;
 *          i++;
 *      clear(recv);
 */

int wait_for_acknowledgement(int socket_desc, struct sockaddr_storage remote_addr, socklen_t addr_len, unsigned char * data) {
    int return_value, i = 0;
    unsigned int seq_num, ack_num;

    unsigned char * cursor;
    unsigned char recv[MAX_BUFFER_SIZE];
    memset(recv, 0, MAX_BUFFER_SIZE);

    cursor = &data[4];
    seq_num = char_to_int(cursor);

    while (i < MAX_RETRIES) {
        return_value = (int)recvfrom(socket_desc, recv, MAX_BUFFER_SIZE, 0, (struct sockaddr *) &remote_addr, &addr_len);
        if (return_value == -1) {
            // resend data
            return_value = (int)sendto(socket_desc, data, MAX_BUFFER_SIZE, 0, (struct sockaddr *) &remote_addr, addr_len);
            if (return_value == -1) {
                printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
                return return_value;
            }
            printf("server -> SEQ %d -> client\n", seq_num);
        } else {
            if (memcmp(recv, "ACK\0", 4) == 0) {        // received and ACK, if correct ack, stop
                cursor = &recv[4];
                ack_num = char_to_int(cursor);
                if (seq_num == ack_num) {
                    printf("server <- ACK %d <- client\n", ack_num);
                    break;
                }
            } else if (memcmp(recv, "SEQ\0", 4) == 0) { // received a SEQ, send ack for SEQ
                cursor = &recv[4];
                ack_num = char_to_int(cursor);

                // send ack for SEQ received
                return_value = send_acknowledgement(socket_desc, remote_addr, addr_len, ack_num);
                if (return_value == -1) {
                    return return_value;
                }

                // resend data
                return_value = (int)sendto(socket_desc, data, MAX_BUFFER_SIZE, 0, (struct sockaddr *) &remote_addr, addr_len);
                if (return_value == -1) {
                    printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
                    return return_value;
                }
                printf("server -> SEQ %d -> client\n", seq_num);
                i = 0;
                continue;
            }
        }
        i++;
    }

    memset(recv, 0, MAX_BUFFER_SIZE);
    return 0;
}

/*
 * send_error_message():
 *      copy "ERR" onto buffer;
 *      copy error message to buffer;
 *      copy error size to buffer;
 *      send buffer to client;
 *      wait_for_acknowledgement();
 */

int send_error_message(int socket_desc, struct sockaddr_storage remote_addr, socklen_t addr_len, char * error_msg) {
    int return_value;
    unsigned char * cursor;
    unsigned char data[MAX_BUFFER_SIZE];
    memset(data, 0, MAX_BUFFER_SIZE);

    memcpy(data, "ERR", 4);
    cursor = &data[8];
    int_to_char(cursor, strlen(error_msg));
    cursor = &data[12];
    memcpy(cursor, error_msg, strlen(error_msg));

    return_value = (int)sendto(socket_desc, data, MAX_BUFFER_SIZE, 0, (struct sockaddr *) &remote_addr, addr_len);
    if (return_value == -1) {
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return return_value;
    }
    printf("server -> ERR %d -> client\n", 0);

    return_value = wait_for_acknowledgement(socket_desc, remote_addr, addr_len, data);
    if (return_value == -1) {
        return return_value;
    }

    memset(data, 0, MAX_BUFFER_SIZE);
    return 0;
}

/*
 * send_file():
 *      if you can open file:
 *          while not at EOF, read from file:
 *              if buffer is not full:
 *                  write data into buffer;
 *              else:
 *                  manage data header info
 *                  send data;
 *                  wait_for_acknowledgement();
 *          at end of while, at this point it has written to buff without sending it;
 *          send buff;
 *      else:
 *          return error; (file can't be opened == -2; doesn't exist == -2)
 *      close file;
 */

int send_file(int socket_desc, struct sockaddr_storage remote_addr, socklen_t addr_len, unsigned int seq_num, unsigned char * file_name) {
    FILE *fp;
    int return_value;
    unsigned int cursorNum = 12;

    unsigned char c;
    unsigned char * cursor;
    unsigned char data[MAX_BUFFER_SIZE];
    memset(data, 0, MAX_BUFFER_SIZE);

    if (access((char *)file_name, F_OK) == 0) {
        // file exists
        if ((fp = fopen((char *)file_name, "r")) == NULL) {
            printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
            return -2;
        }

        while (fread(&c, 1, 1, fp) == 1) {

            if (cursorNum < MAX_BUFFER_SIZE-1) {
                cursor = &data[cursorNum];
                memcpy(cursor, &c, 1);

            } else {
                cursor = &data[cursorNum];
                memcpy(cursor, &c, 1);

                memcpy(data, "SEQ", 4);
                seq_num++;
                cursor = &data[4];
                int_to_char(cursor, seq_num);
                cursor = &data[8];
                int_to_char(cursor, (cursorNum+1)-12);

                return_value = (int)sendto(socket_desc, data, cursorNum+12, 0, (struct sockaddr *) &remote_addr, addr_len);
                if (return_value == -1) {
                    printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
                    return return_value;
                }
                printf("server -> SEQ %d -> client\n", seq_num);

                return_value = wait_for_acknowledgement(socket_desc, remote_addr, addr_len, data);
                if (return_value == -1) {
                    return return_value;
                }

                memset(data, 0, MAX_BUFFER_SIZE);
                cursorNum = 12;
                continue;
            }
            cursorNum++;
        }
        // completely finished reading the file, stored it in last buffer, send remaining data.
        memcpy(data, "SEQ", 4);
        seq_num++;
        cursor = &data[4];
        int_to_char(cursor, seq_num);
        cursor = &data[8];
        int_to_char(cursor, cursorNum-12);

        return_value = (int)sendto(socket_desc, data, cursorNum+12, 0, (struct sockaddr *) &remote_addr, addr_len);
        if (return_value == -1) {
            printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
            return return_value;
        }
        printf("server -> SEQ %d -> client\n", seq_num);
        wait_for_acknowledgement(socket_desc, remote_addr, addr_len, data);

        memset(data, 0, MAX_BUFFER_SIZE);

    } else {
        //file doesn't exist
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return -2;
    }
    fclose(fp);

    memset(data, 0, MAX_BUFFER_SIZE);
    return 0;
}

/*
 * handle_connection():
 *      recv data;
 *      if data == SEQ:
 *          store SEQ num;
 *          send_acknowledgement();
 *          read file name;
 *          send_file();
 *          if it fails to open file:
 *              send ERR to client saying "file not found!";
 *      else:
 *          don't respond, or respond with an ERR saying "bad request" or something;
 */

int handle_connection(int socket_desc) {
    int return_value;
    unsigned int seq_num;

    struct sockaddr_storage remote_addr;
    socklen_t addr_len;
    addr_len = sizeof(remote_addr);

    unsigned char * cursor;
    unsigned char recv[MAX_BUFFER_SIZE];
    memset(recv, 0, MAX_BUFFER_SIZE);

    while (1) {
        return_value = (int)recvfrom(socket_desc, recv, MAX_BUFFER_SIZE, 0, (struct sockaddr *) &remote_addr, &addr_len);
        if (return_value == -1) {
            continue;
        } else {
            break;
        }
    }
    // if data is SEQ
    if (memcmp(recv, "SEQ\0", 4) == 0) {
        // get seq num
        cursor = &recv[4];
        seq_num = char_to_int(cursor);
        printf("server <- SEQ %d <- client\n", seq_num);


        return_value = send_acknowledgement(socket_desc, remote_addr, addr_len, seq_num);
        if (return_value == -1) {
            return return_value;
        }

        cursor = &recv[12];     // file name at this location
        return_value = send_file(socket_desc, remote_addr, addr_len, seq_num, cursor);
        if (return_value == -1) {
            return return_value;
        } else if (return_value == -2) {
            return_value = send_error_message(socket_desc, remote_addr, addr_len, "File Doesn't Exist!");
            if (return_value == -1) {
                return return_value;
            } else {
                return -2;
            }
        }
    } else {
        return_value = send_error_message(socket_desc, remote_addr, addr_len, "Bad Request!");
        if (return_value == -1) {
            return return_value;
        } else {
            return -2;
        }
    }

    memset(recv, 0, MAX_BUFFER_SIZE);
    return 0;
}

int main(int argc, char *argv[]) {
    int return_value;
    char * MY_PORT;

    int socket_desc;
    struct addrinfo hints, *servInfo, *p;

    // command line arguments
    if (argc != 2) {
        printf("Argument expected: <Server Port>\n");
        return -2;
    } else {
        MY_PORT = argv[1];
    }

    memset(&hints, 0, sizeof(hints)); // set all data in struct to 0
    hints.ai_family = AF_INET;           // IPv4
    hints.ai_socktype = SOCK_DGRAM;      // UDP
    hints.ai_flags = AI_PASSIVE;         // Listen

    // set timer
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    return_value = getaddrinfo(NULL, MY_PORT, &hints, &servInfo);
    if (return_value != 0) {
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return return_value;
    }

    // socket(): creates a new socket, no address was assigned yet
    for (p = servInfo; p != NULL; p = p->ai_next) {
        socket_desc = socket(
                p->ai_family,    // IPv4
                p->ai_socktype,  // Streaming Protocol
                p->ai_protocol   // UDP
        );
        if (socket_desc == -1) {
            printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
            continue;
        }
        return_value = bind(socket_desc, p->ai_addr, p->ai_addrlen);
        if (return_value == -1) {
            close(socket_desc);
            printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
            continue;
        }

        break;
    }

    return_value = setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval));
    if (return_value == -1 && p == NULL) {
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return return_value;
    }

    freeaddrinfo(servInfo);

    return_value = handle_connection(socket_desc);
    close(socket_desc);

    return return_value;
}
