#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *message = "Hello from client";
    char buffer[1024] = {0};

    // Tworzenie gniazda
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Konwersja adresu IP serwera
    if (inet_pton(AF_INET, "192.168.1.100", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        return -1;
    }

    // Łączenie z serwerem
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }

    send(sock, message, strlen(message), 0);
    printf("Message sent to server\n");

    read(sock, buffer, 1024);
    printf("Message from server: %s\n", buffer);

    close(sock);
    return 0;
}
