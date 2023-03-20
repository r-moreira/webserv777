//
// Created by rmore on 16/03/2023.
//

#include <iostream>
#include <unistd.h>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fstream>

int main() {
    int sockfd = -1, ret = -1;
    struct sockaddr_in server_addr;
    char buf[20];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket failed");
        return EXIT_FAILURE;
    }
    printf("CLIENT:: Socket created successfully\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("Connection failed");
        close(sockfd);
        return EXIT_FAILURE;
    }
    printf("CLIENT:: Connected to server %s:%d\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

    char tmp[20];
    scanf("%s", tmp);

    strncpy(buf, "Hello", 6);
    write(sockfd, buf, strlen(buf) + 1);
    printf("CLIENT:: Data sent to server: %s\n", buf);

    memset(buf, '\0', sizeof(buf));
    read(sockfd, buf, sizeof(buf));
    printf("CLIENT:: Data received from server: %s\n", buf);

    printf("CLIENT:: Communication completed: Closing connection\n");

    close(sockfd);
    return EXIT_SUCCESS;
}