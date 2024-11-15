#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>        // Include this for read, write, and close
#include <netdb.h>
#include <netinet/in.h>

#define PORTNO 5001
#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, clilen;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // Initialize socket structure
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = PORTNO;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind the host address
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        close(sockfd);
        exit(1);
    }

    // Start listening for the clients
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    // Accept actual connection from the client
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) {
        perror("ERROR on accept");
        close(sockfd);
        exit(1);
    }

    printf("start\n");

    // Communication loop
    while (1) {
        bzero(buffer, BUFFER_SIZE);
        n = read(newsockfd, buffer, BUFFER_SIZE - 1);

        if (n < 0) {
            perror("ERROR in reading from socket");
            close(newsockfd);
            close(sockfd);
            exit(1);
        }

        printf("client said: %s \n", buffer);

        n = write(newsockfd, buffer, strlen(buffer));
        if (n < 0) {
            perror("ERROR in writing to socket");
            close(newsockfd);
            close(sockfd);
            exit(1);
        }

        // Exit if the client sends "quit"
        if (strncmp(buffer, "quit", 4) == 0)
            break;
    }

    // Close sockets
    close(newsockfd);
    close(sockfd);
    return 0;
}
