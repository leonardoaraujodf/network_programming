#include <iostream>
#include <cstdlib>
#include "net_adapters.h"

NetworkAdapters::NetworkAdapters() {
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        std::cerr << "Failed to initialize!\n";
        exit(EXIT_FAILURE);
    }

    DWORD asize = 20000;
    do {
        adapters = (PIP_ADAPTER_ADDRESSES)malloc(asize);
        if (!adapters) {
            std::cerr << "Could not allocate" << asize;
            std::cerr << " bytes for adapters!\n";
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        int r = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, adapters, &asize);
        if (r == ERROR_BUFFER_OVERFLOW) {
            std::cout << "GetAdaptersAddresses wants " << asize << " bytes\n";
            free(adapters);
        } else if (r == ERROR_SUCCESS) {
            break;
        } else {
            std::cerr << "Error from GetAdaptersAddresses: " << r << " \n";
            WSACleanup();
            exit(EXIT_FAILURE);
        }
    } while (!adapters);
}

NetworkAdapters::~NetworkAdapters() {
    WSACleanup();
}

void NetworkAdapters::list(void) {
    for (auto it = begin(); it != end(); ++it) {
        PIP_ADAPTER_ADDRESSES adapter = *it;
        std::wcout << "\nAdapter name: " << adapter->FriendlyName << '\n';
        PIP_ADAPTER_UNICAST_ADDRESS address = adapter->FirstUnicastAddress;
        while (address) {
            std::cout << '\t';
            if (address->Address.lpSockaddr->sa_family == AF_INET) {
                std::cout << "IPv4\n";
            } else {
                std::cout << "IPv6\n";
            }
            char ap[100];
            getnameinfo(address->Address.lpSockaddr, address->Address.iSockaddrLength, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
            std::cout << '\t' << ap << '\n';
            address = address->Next;
        }
    }
}
