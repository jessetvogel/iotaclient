#include "socket.hpp"
#include <unistd.h>
#include <fstream>

#define FILE_BUFFER_SIZE (512)
#define DATA_BUFFER_SIZE (512)
#define REQUEST_BUFFER_SIZE (512)

bool Socket::write(int socket, std::string string) {
    size_t length = string.length();
    return ::write(socket, string.c_str(), length) == length;
}

bool Socket::writeLine(int socket) {
    return write(socket, "\n");
}

bool Socket::writeLine(int socket, std::string string) {
    return write(socket, string) && writeLine(socket);
}

bool Socket::writeFile(int socket, std::string path) {
    std::ifstream file;
    file.open(path);
    if(file.fail()) return false;
    
    char buffer[FILE_BUFFER_SIZE];
    std::streamsize bytes;
    do {
        file.read(buffer, FILE_BUFFER_SIZE);
        bytes = file.gcount();
        ::write(socket, buffer, bytes);
    } while(bytes > 0);
    
    file.close();
    return true;
}

bool Socket::writeData(int socket, void* data, int bytes) {
    char* ptr = (char*) data;
    while(bytes > 0) {
        int bufferSize = bytes > DATA_BUFFER_SIZE ? DATA_BUFFER_SIZE : bytes;
        ssize_t n = ::write(socket, ptr, bufferSize);
        if(n < 1) return false;
        ptr += n;
        bytes -= n;
    }
    return true;
}

int Socket::read(int socket, void* data, int bytes) {
    char* ptr = (char*) data;
    int N = 0;
    do {
        ssize_t n = ::read(socket, ptr, bytes > DATA_BUFFER_SIZE ? DATA_BUFFER_SIZE : bytes);
        if(n < 1) break;
        N += n;
        ptr += n;
        bytes -= n;
    } while(bytes > 0);
    
    return N;
}

std::string Socket::readLine(int socket) {
    char buffer[REQUEST_BUFFER_SIZE];
    char* ptr = buffer;
    ssize_t bytes;
    do bytes = ::read(socket, ptr, 1);
    while(bytes > 0 && *(ptr++) != '\n');
    long length = ptr - buffer - 2;
    if(length < 0) length = 0;
    return std::string(buffer, length);
}
