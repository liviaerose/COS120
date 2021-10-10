#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/time.h>

#define MAX_BUFFER_SIZE 1458
#define MAX_RETRIES 8

/*
 * I have been mulling over the code, and I keep getting lost in all of the details that we have to keep track of.
 * I think it will be easier if we write some pseudocode of what needs to be done before we do it. That way we have
 * something we can look at as a model in our code and go from there. So, here is some pseudocode that I have made
 * for client.c. This is not necessary to follow, but I think a great place to start before we get too lost in our
 * own code. I know I feel that way, and maybe so do you. So, here it is:
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
 * wait_for_acknowledgement():
 *      seq_num = char_to_int(data);
 *      while(i < MAX_RETRIES):
 *          wait for 2 seconds;
 *              recv ack;
 *          if no recv in 2 seconds:
 *              send data again;
 *          else:
 *              ack_num = char_to_data(recv);
 *              if (seq_num == ack_num:
 *                  break;
 *          i++;
 *      clear(recv);
 *
 * send_acknowledgement():
 *      make ACK header with ACK STR and ACK NUM from SEQ num;
 *      send header to server;
 *
 * handle_connection():
 *      make SEQ header with SEQ STR, SEQ NUM (1), and data size. add server file to request;
 *      send file request;
 *      wait_for_acknowledgement();
 *
 *      while(1):
 *          recv data;
 *          if data == SEQ:
 *              store SEQ num;
 *              send_acknowledgement();
 *              read data;
 *              write data into local file;
 *      close file;
 *
 * main():
 *      open socket;
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
 *      send header to server;
 */

int send_acknowledgement(int socket_desc, struct addrinfo * p, unsigned int ack_num) {
    int return_value;
    unsigned char * cursor;
    unsigned char data[8];
    memset(data, 0, sizeof(data));

    memcpy(data, "ACK", 4);
    cursor = &data[4];
    int_to_char(cursor, ack_num);

    return_value = (int)sendto(socket_desc, data, sizeof(data), 0, p->ai_addr, p->ai_addrlen);
    if (return_value == -1) {
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return return_value;
    }
    printf("client -> ACK %d -> server\n", ack_num);

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
 *              if (seq_num == ack_num:
 *                  break;
 *          i++;
 *      clear(recv);
 */

int wait_for_acknowledgement(int socket_desc, struct addrinfo * p, unsigned char * data) {
    int return_value, i = 0;
    unsigned int seq_num, ack_num;

    unsigned char * cursor;
    unsigned char recv[MAX_BUFFER_SIZE];
    memset(recv, 0, MAX_BUFFER_SIZE);

    cursor = &data[4];
    seq_num = char_to_int(cursor);

    while(i < MAX_RETRIES) {
        return_value = (int)recvfrom(socket_desc, recv, MAX_BUFFER_SIZE, 0, (struct sockaddr *) p->ai_addr, &p->ai_addrlen);
        if (return_value == -1) {
            // resend data
            return_value = (int)sendto(socket_desc, data, MAX_BUFFER_SIZE, 0, (struct sockaddr *) p->ai_addr, p->ai_addrlen);
            if (return_value == -1) {
                printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
                return return_value;
            }
            printf("client -> SEQ %d -> server\n", seq_num);
        } else {
            if (memcmp(recv, "ACK\0", 4) == 0) {
                cursor = &recv[4];
                ack_num = char_to_int(cursor);
                if (seq_num == ack_num) {
                    printf("client <- ACK %d <- server\n", ack_num);
                    break;
                }
            }
        }
        i++;
    }
    if (i >= MAX_RETRIES) {
        printf("Error: Connection Closed.\n");
        return -1;
    }
    memset(recv, 0, MAX_BUFFER_SIZE);
    return 0;

}

/*
 * handle_connection():
 *      make SEQ header with SEQ STR, SEQ NUM (1), and data size. add server file to request;
 *      send file request;
 *      wait_for_acknowledgement();
 *
 *      while(1):
 *          recv data;
 *          if data == SEQ:
 *              store SEQ num;
 *              send_acknowledgement();
 *              read data;
 *              write data into local file;
 *      close file;
 */

int handle_connection(int socket_desc, struct addrinfo * p, char * remote_file, char * local_file) {
    FILE *fp;
    int return_value, i = 0;
    unsigned int temp, seq_num = 1;
    unsigned int size;

    unsigned char * cursor;
    unsigned char data[MAX_BUFFER_SIZE];
    unsigned char recv[MAX_BUFFER_SIZE];
    memset(data, 0, MAX_BUFFER_SIZE);
    memset(recv, 0, MAX_BUFFER_SIZE);

    // make first seq header with requesting file
    memcpy(data, "SEQ", 4);
    cursor = &data[4];
    int_to_char(cursor, seq_num);
    cursor = &data[8];
    int_to_char(cursor, strlen(remote_file));
    cursor = &data[12];
    memcpy(cursor, remote_file, strlen(remote_file));

    return_value = (int)sendto(socket_desc, data, strlen(remote_file)+12, 0, p->ai_addr, p->ai_addrlen);
    if (return_value == -1) {
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return return_value;
    }
    printf("client -> SEQ %d -> server\n", seq_num);

    return_value = wait_for_acknowledgement(socket_desc, p, data);
    if (return_value == -1) {
        return return_value;
    }

    if ((fp = fopen(local_file, "w")) == NULL) {
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return -1;
    }

    while (i < MAX_RETRIES) {
        return_value = (int)recvfrom(socket_desc, recv, MAX_BUFFER_SIZE, 0, p->ai_addr, &p->ai_addrlen);
        if (return_value == -1) {
            i++;
        } else {
            i = 0;
            // if data is SEQ
            if (memcmp(recv, "SEQ\0", 4) == 0) {
                // get seq num
                cursor = &recv[4];
                temp = char_to_int(cursor);
                printf("client <- SEQ %d <- server\n", temp);
                if (seq_num == temp) {  // server didn't receive ACK, resend last ACK
                    return_value = send_acknowledgement(socket_desc, p, seq_num);
                    if (return_value == -1) {
                        return return_value;
                    }
                } else {
                    seq_num = temp;
                }

                return_value = send_acknowledgement(socket_desc, p, seq_num);
                if (return_value == -1) {
                    return return_value;
                }
                cursor = &recv[8];
                size = char_to_int(cursor);
                cursor = &recv[12];
                fwrite(cursor, size, 1, fp);

            } else if (memcmp(recv, "ERR\0", 4) == 0) {
                // handle a server file opening error
                cursor = &recv[4];
                seq_num = char_to_int(cursor);
                printf("client <- ERR %d <- server\n", seq_num);
                return_value = send_acknowledgement(socket_desc, p, seq_num);
                if (return_value == -1) {
                    return return_value;
                }
                cursor = &recv[12];
                printf("Error: %s (line: %d)\n", cursor, __LINE__);

                break;
            }
        }
    }
    if (i >= MAX_RETRIES) {
        printf("Error: Connection Closed.\n");
    }
    fclose(fp);

    memset(data, 0, MAX_BUFFER_SIZE);
    memset(recv, 0, MAX_BUFFER_SIZE);
    return 0;
}

int main(int argc, char *argv[]) {
    int return_value;
	char * SERVER_IP, * SERVER_PORT, * REMOTE_PATH, * LOCAL_PATH;
    char file_name[24] = "/";
    char * file_name_ptr = &file_name[1];

	int socket_desc;
	struct addrinfo hints, *servInfo, *p;

	// command line arguments
	if (argc != 5) {
        printf("Arguments expected: <Server IP> <Server Port> <Remote Path> <Local Path>\n");
        return -2;
    } else {
        SERVER_IP = argv[1];
        SERVER_PORT = argv[2];
        REMOTE_PATH = argv[3];
        LOCAL_PATH = argv[4];
	}

	memset(&hints, 0, sizeof(hints));// set all data in struct to 0
	hints.ai_family = AF_INET;          // IPv4
	hints.ai_socktype = SOCK_DGRAM;     // UDP

	// set timer
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;


    return_value = getaddrinfo(SERVER_IP, SERVER_PORT, &hints, &servInfo);
    if (return_value != 0) {
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return return_value;
    }

    for (p = servInfo; p != NULL; p = p->ai_next) {
        // socket(): creates a new socket, no address was assigned yet
        socket_desc = socket(
                p->ai_family,    // IPv4
                p->ai_socktype,  // Streaming Protocol
                p->ai_protocol   // UDP
        );
        if (socket_desc == -1) {
            printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
            continue;
        }

        break;
    }

    return_value = setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *) &tv, sizeof(struct timeval));
    if (return_value == -1 && p == NULL) {
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return return_value;
    }

    printf("Enter a file name: ");
    scanf("%s", file_name_ptr);
    char remote_file[strlen(REMOTE_PATH)+strlen(file_name)+1];
    char local_file[strlen(LOCAL_PATH)+strlen(file_name)+1];
    memset(remote_file, 0, sizeof(remote_file));
    memset(local_file, 0, sizeof(local_file));

    strncat(remote_file, REMOTE_PATH, strlen(REMOTE_PATH));
    strncat(remote_file, file_name, strlen(file_name));
    strncat(local_file, LOCAL_PATH, strlen(LOCAL_PATH));
    strncat(local_file, file_name, strlen(file_name));

    freeaddrinfo(servInfo);

    return_value = handle_connection(socket_desc, p, remote_file, local_file);
    close(socket_desc);

    return return_value;
}
