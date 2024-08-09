#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <cstdlib>
#include <iostream>
#include "net_adapters.h"

NetworkAdapters::NetworkAdapters() {
    if (getifaddrs(&addresses) == -1) {
        std::cerr << "getifaddrs call failed\n";
        std::exit(EXIT_FAILURE);
    }
}

NetworkAdapters::~NetworkAdapters() {
    freeifaddrs(addresses);
}

void NetworkAdapters::list(void) {
    for (auto it = begin(); it != end(); ++it) {
        struct ifaddrs* addr = *it;
        int family = addr->ifa_addr->sa_family;
        if (family == AF_INET || family == AF_INET6) {
            std::cout << addr->ifa_name << "\t";
            if (family == AF_INET) {
                std::cout << "IPv4";
            } else {
                std::cout << "IPv6";
            }
            std::cout << '\t';
            char ap[100];
            const int family_size = family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
            getnameinfo(addr->ifa_addr, family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
            std::cout << "\t" << ap << "\n";
        }
        addr = addr->ifa_next;
    }
}
