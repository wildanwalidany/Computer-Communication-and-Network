#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>        // Include this header for read and write
#include <netdb.h>
#include <netinet/in.h>

#define PORTNO 5001
#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[BUFFER_SIZE];

    // create socket and check for error
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // get server details
    server = gethostbyname("127.0.0.1");
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(1);
    }

    // setup server address structure
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(PORTNO);

    // connect to the server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR while connecting");
        exit(1);
    }

    // communication loop
    while (1) {
        printf("What do you want to say? ");
        bzero(buffer, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE - 1, stdin);

        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            perror("ERROR while writing to socket");
            exit(1);
        }

        bzero(buffer, BUFFER_SIZE);
        n = read(sockfd, buffer, BUFFER_SIZE - 1);
        if (n < 0) {
            perror("ERROR while reading from socket");
            exit(1);
        }
        printf("server replied: %s", buffer);

        // exit if the server sends "quit"
        if (strncmp(buffer, "quit", 4) == 0)
            break;
    }

    close(sockfd);
    return 0;
}
