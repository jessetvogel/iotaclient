#include "request.hpp"
#include "socket.hpp"
#include <unistd.h>

#define REGEX_METHOD "GET|POST"
#define REGEX_REQUEST_URI "\\S+"
#define REGEX_HTTP_VERSION "HTTP\\/1\\.1"

std::regex Request::regexRequestLine("^(" REGEX_METHOD ") (" REGEX_REQUEST_URI ") (" REGEX_HTTP_VERSION ")$");
std::regex Request::regexRequestURI("^(\\/.*?)(?:\\?(.*))?$");

bool Request::receive(int socket) {
    // Store socket
    this->socket = socket;

    // Read request line
    std::cmatch cm;
    std::string line = Socket::readLine(socket);
    if(!std::regex_search(line.c_str(), cm, regexRequestLine)) return false;
    
    method = cm[1];
    requestURI = cm[2];
    httpVersion = cm[3];
    
    // Decode request URI     // TODO properly decode url
    for(int i = 0;i < requestURI.length(); ++i) if(requestURI[i] == '+') requestURI[i] = ' ';
    
    // Parse URI
    if(!std::regex_search(requestURI.c_str(), cm, regexRequestURI)) return false;
    requestPath = cm[1];
    requestQuery = cm[2];
    
    // May not contain '..'
    if(requestURI.find("..") != std::string::npos) return false;
    
    // Read headers
    if(!headers.receive(socket)) return false;
    
    return true;
}

int Request::getData(void* data, int bytes) {
    return Socket::read(socket, data, bytes);
}

bool Request::sendRequestLine() {
    return Socket::writeLine(socket, method + " " + requestURI + " " + httpVersion);
}

bool Request::sendHeaders() {
    return headers.send(socket);
}

bool Request::sendLine() {
    return Socket::writeLine(socket);
}


bool Request::sendData(void* data, int bytes) {
    return Socket::writeData(socket, data, bytes);
}
