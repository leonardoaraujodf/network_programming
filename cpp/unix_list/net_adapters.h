#pragma once

#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>

class Iterator {
public:
    Iterator(struct ifaddrs* ptr) : current(ptr) {}

    bool operator!=(const Iterator& other) const {
        return current != other.current;
    }

    Iterator& operator++() {
        if (current) {
            current = current->ifa_next;
        }
        return *this;
    }

    struct ifaddrs* operator*() const {
        return current;
    }

private:
    struct ifaddrs* current;
};

class NetworkAdapters {
public:
    NetworkAdapters();
    ~NetworkAdapters();
    void list(void);
    Iterator begin() const {
        return Iterator(addresses);
    }

    Iterator end() const {
        return Iterator(nullptr);
    }
private:
    struct ifaddrs *addresses;
};
