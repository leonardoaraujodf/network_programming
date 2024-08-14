#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x6000
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#endif

#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())

#if !defined(IPV6_V6ONLY)
#define IPV6_V6ONLY 27
#endif

#else
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) (close(s))
#define SOCKET int
#define GETSOCKETERRNO() (errno)
#endif

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>

int main() {
#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        std::cerr << "Failed to initialize.\n";
        exit(EXIT_FAILURE)
    }
#endif
    std::cout << "Configuring local address...\n";
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bindAddress;
    getaddrinfo(0, "8080", &hints, &bindAddress);

    std::cout << "Creating socket...\n";
    SOCKET socketListen;
    socketListen = socket(bindAddress->ai_family,
            bindAddress->ai_socktype, bindAddress->ai_protocol);
    if (!ISVALIDSOCKET(socketListen)) {
        std::cerr << "socket() failed. (" << GETSOCKETERRNO() << ")\n";
        exit(EXIT_FAILURE);
    }

    int option = 0;
    if (setsockopt(socketListen, IPPROTO_IPV6, IPV6_V6ONLY, (void*)&option, sizeof(option))) {
        std::cerr << "setsockopt() failed. (" << GETSOCKETERRNO() << ")\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Binding socket to local address...\n";
    if (bind(socketListen,
                bindAddress->ai_addr, bindAddress->ai_addrlen)) {
        std::cerr << "bind() failed. (" << GETSOCKETERRNO() << ")\n";
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(bindAddress);

    std::cout << "Listening...\n";
    if (listen(socketListen, 10) < 0) {
        std::cerr << "listen() failed. (" << GETSOCKETERRNO() << ")\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Waiting for connection...\n";
    struct sockaddr_storage clientAddress;
    socklen_t clientLen = sizeof(clientAddress);
    SOCKET socketClient = accept(socketListen,
            (struct sockaddr*)&clientAddress, &clientLen);
    if (!ISVALIDSOCKET(socketClient)) {
        std::cerr << "accept() failed. (" << GETSOCKETERRNO() << ")\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Client is connected... ";
    char addressBuffer[100];
    getnameinfo((struct sockaddr*)&clientAddress,
            clientLen, addressBuffer, sizeof(addressBuffer), 0, 0,
            NI_NUMERICHOST);
    std::cout << addressBuffer << '\n';

    std::cout << "Reading request...\n";
    char request[1024];
    int bytesReceived = recv(socketClient, request, 1024, 0);
    std::cout << "Received " << bytesReceived << " bytes.\n";

    std::cout << "Sending response...\n";
    const std::string response = 
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Content-Type: text/plain\r\n\r\n"
        "Local time is: ";
    int bytesSent = send(socketClient, response.c_str(), response.size(), 0);
    std::cout << "Sent " << bytesSent << " of " << (int)response.size() << "bytes.\n";

    time_t timer;
    time(&timer);
    char *timeMsg = ctime(&timer);
    bytesSent = send(socketClient, timeMsg, strlen(timeMsg), 0);
    std::cout << "Sent " << bytesSent << " of " << strlen(timeMsg) << "bytes.\n";

    std::cout << "Closing connection...\n";
    CLOSESOCKET(socketClient);
    std::cout << "Closing listening socket...\n";
    CLOSESOCKET(socketListen);

#if defined(_WIN32)
    WSACleanup();
#endif
    std::cout << "Finished.\n";
    
    return 0;
}
