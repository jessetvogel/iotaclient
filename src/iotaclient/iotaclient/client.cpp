#include "client.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "log.hpp"

#define NO_SOCKET (-1)

Client::~Client() {
    if(request != NULL) delete request;
    if(response != NULL) delete response;
    if(socket != NO_SOCKET) close(socket);
}

Request* Client::createRequest() {
    // If response was already created, stop
    if(response != NULL) {
        Log::error("Response was already read");
        return NULL;
    }
    
    // If a request was already created, return it
    if(request != NULL)
        return request;
    
    // Create hints
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Get server address info
    struct addrinfo* serverInfo;
    if(getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &serverInfo) != 0) {
        Log::error("Failed to getaddrinfo");
        return NULL;
    }
    
    // Try to create socket and connect
    struct addrinfo* ptr;
    for(ptr = serverInfo;ptr != NULL;ptr = ptr->ai_next) {
        if((socket = ::socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) < 0)
            continue;
        
        if(::connect(socket, ptr->ai_addr, ptr->ai_addrlen) < 0) {
            close(socket);
            continue;
        }

        // Succesfully connected
        break;
    }
    
    if(ptr == NULL) {
        Log::error("Couldn't connect to the server");
        socket = NO_SOCKET;
        return NULL;
    }
    
    freeaddrinfo(serverInfo);
    
    // Create request object
    return (request = new Request(socket));
}

Response* Client::getResponse() {
    // If no request was created, stop
    if(request == NULL) {
        Log::error("Request was not yet made");
        return NULL;
    }
    
    // If response was already created, return it
    if(response != NULL)
        return response;
    
    // Make sure a request was created
    if(socket == NO_SOCKET) {
        Log::error("No request was created");
        return NULL;
    }
    
    // Create response object and receive
    Response* response = new Response();
    response->receive(socket);
    return response;
}
