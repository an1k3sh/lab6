#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;
    char sendline[512];
    char recvline[512];
    ssize_t n;

    if (argc != 2) {
        printf("Usage: %s <ipaddress>\n", argv[0]);
        exit(1);
    }

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(6000);
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connect failed");
        exit(1);
    }

    printf("Enter a message to send to the server:\n");
    fgets(sendline, sizeof(sendline), stdin);
    n = strlen(sendline);

    // Send the message to the server
    write(sockfd, sendline, n);

    // Receive and print the server's response
    n = read(sockfd, recvline, sizeof(recvline));
    if (n < 0) {
        perror("Error reading from server");
        exit(1);
    }

    recvline[n] = 0;
    printf("Server response: %s\n", recvline);

    close(sockfd);
    return 0;
}
