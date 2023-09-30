#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct ip *ip_header;
    struct icmphdr *icmp_header;
    char buffer[1024];

    // Create a raw socket to capture ICMP packets
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    while (1) {
        ssize_t packet_len = recv(sockfd, buffer, sizeof(buffer), 0);
        if (packet_len < 0) {
            perror("Packet reception error");
            close(sockfd);
            exit(1);
        }

        ip_header = (struct ip *)buffer;
        icmp_header = (struct icmphdr *)(buffer + (ip_header->ip_hl << 2));

        printf("IP Header Length: %d bytes\n", ip_header->ip_hl << 2);
    }

    close(sockfd);
    return 0;
}
