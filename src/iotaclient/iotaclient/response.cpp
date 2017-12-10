#include "response.hpp"
#include "socket.hpp"

#define REGEX_HTTP_VERSION "HTTP\\/1\\.1"
#define REGEX_STATUS "[\\w\\s]+"

std::regex Response::regexStatus("^(" REGEX_HTTP_VERSION ") (" REGEX_STATUS ")$");

bool Response::receive(int socket) {
    // Store socket
    this->socket = socket;
    
    // Read status line
    std::cmatch cm;
    std::string line = Socket::readLine(socket);
    if(!std::regex_search(line.c_str(), cm, regexStatus)) return false;
    
    httpVersion = cm[1];
    status = cm[2];
    
    // Read headers
    if(!headers.receive(socket)) return false;
    
    return false;
}

int Response::getData(void* data, int bytes) {
    return Socket::read(socket, data, bytes);
}

bool Response::sendStatus() {
    return Socket::write(socket, "HTTP/1.1 ") && Socket::writeLine(socket, status);
}

bool Response::sendHeaders() {
    return headers.send(socket);
}

bool Response::sendLine(std::string line) {
    return Socket::writeLine(socket, line);
}

bool Response::sendFile(std::string path) {
    return Socket::writeFile(socket, path);
}

bool Response::sendData(void* data, int bytes) {
    return Socket::writeData(socket, data, bytes);
}
