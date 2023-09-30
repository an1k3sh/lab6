#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

void print_ip_header(struct ip *ip_header) {
    printf("IP Header\n");
    printf("  Version: %d\n", ip_header->ip_v);
    printf("  Header Length: %d bytes\n", (ip_header->ip_hl) * 4);
    printf("  Type of Service: %d\n", ip_header->ip_tos);
    printf("  Total Length: %d bytes\n", ntohs(ip_header->ip_len));
    printf("  Identification: %d\n", ntohs(ip_header->ip_id));
    printf("  Fragment Offset: %d\n", ntohs(ip_header->ip_off) & IP_OFFMASK);
    printf("  Time To Live (TTL): %d\n", ip_header->ip_ttl);
    printf("  Protocol: %d\n", ip_header->ip_p);
    printf("  Checksum: %d\n", ntohs(ip_header->ip_sum));
    printf("  Source IP: %s\n", inet_ntoa(ip_header->ip_src));
    printf("  Destination IP: %s\n", inet_ntoa(ip_header->ip_dst));
}

int main() {
    int sockfd;
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

        struct ip *ip_header = (struct ip *)buffer;
        
        printf("\nReceived an ICMP packet:\n");
        print_ip_header(ip_header);

        // Print ICMP payload data (if any)
        int ip_header_len = (ip_header->ip_hl) * 4;
        int payload_len = packet_len - ip_header_len;
        if (payload_len > 0) {
            printf("ICMP Payload Data:\n");
            for (int i = ip_header_len; i < packet_len; i++) {
                printf("%02x ", buffer[i]);
                if ((i - ip_header_len + 1) % 16 == 0)
                    printf("\n");
            }
            printf("\n");
        }
    }

    close(sockfd);
    return 0;
}
