#pragma once

// #ifndef _WIN32_WINNT
// #define _WIN32_WINNT 0x0600
// #endif

#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>

class Iterator {
public:
    Iterator(PIP_ADAPTER_ADDRESSES ptr) : current(ptr) {}

    bool operator!=(const Iterator& other) const {
        return current != other.current;
    }

    Iterator& operator++() {
        if (current) {
            current = current->Next;
        }
        return *this;
    }

    PIP_ADAPTER_ADDRESSES operator*() const {
        return current;
    }

private:
    PIP_ADAPTER_ADDRESSES current;
};

class NetworkAdapters {
public:
    NetworkAdapters();
    ~NetworkAdapters();
    void list(void);
    Iterator begin() const {
        return Iterator(adapters);
    }

    Iterator end() const {
        return Iterator(nullptr);
    }
private:
    PIP_ADAPTER_ADDRESSES adapters;
};
