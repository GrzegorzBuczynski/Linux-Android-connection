Share


You said:
bezposrednia komunikacja miedzy pc (ubuntu) a android. jak bezprzewodowo przesylac dane w dwie strony? Jak napisac program w c?

ChatGPT said:
ChatGPT
Aby stworzyć program do bezpośredniej, bezprzewodowej komunikacji między komputerem z Ubuntu a urządzeniem z Androidem w języku C, można wykorzystać popularne protokoły komunikacji, takie jak Wi-Fi, Bluetooth lub gniazda sieciowe (sockets). Oto przegląd kroków i możliwości:

1. Wybór metody komunikacji
a) Wi-Fi (TCP/UDP Sockets)
Tworzenie serwera na komputerze (lub Androidzie) i klienta na drugim urządzeniu.
Gniazda TCP (dla niezawodności) lub UDP (dla wydajności, np. w grach).
Protokół HTTP/REST może być nadbudowany na górze gniazd TCP.
b) Bluetooth (RFCOMM)
Wymaga modułów Bluetooth na obu urządzeniach.
Na Ubuntu można użyć bibliotek, takich jak bluez.
Na Androidzie można korzystać z Bluetooth API w Java/Kotlin.
2. Przygotowanie połączenia Wi-Fi za pomocą gniazd
Komunikacja z użyciem gniazd (sockets) jest najprostszym podejściem do przesyłania danych przez Wi-Fi. Poniżej znajduje się uproszczony przykład programu w C.

Serwer na Ubuntu
c
Copy code
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char *message = "Hello from server";

    // Tworzenie gniazda
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Konfiguracja adresu
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bindowanie gniazda
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Nasłuchiwanie na połączenia
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is waiting for a connection...\n");

    // Akceptowanie połączenia
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Odczyt danych od klienta
    read(new_socket, buffer, 1024);
    printf("Message from client: %s\n", buffer);

    // Wysłanie odpowiedzi
    send(new_socket, message, strlen(message), 0);
    printf("Response sent to client\n");

    close(new_socket);
    close(server_fd);
    return 0;
}
Klient na Androidzie (NDK lub JNI)
Program w C w Androidzie można napisać z użyciem Android NDK. Użycie standardowych gniazd TCP/UDP jest również możliwe.

Przykład klienta TCP:

c
Copy code
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
