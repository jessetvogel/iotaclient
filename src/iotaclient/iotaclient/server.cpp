#include "server.hpp"
#include "request.hpp"
#include "response.hpp"
#include "log.hpp"

bool Server::start() {
    // Open socket
    socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if(socket < 0) {
        Log::error("Failed to open socket");
        return false;
    }
    
    // Initialize socket structures
    memset(&serverSocketInfo, 0, sizeof(serverSocketInfo));
    serverSocketInfo.sin_family = AF_INET;
    serverSocketInfo.sin_addr.s_addr = INADDR_ANY;
    serverSocketInfo.sin_port = htons(port);
    
    // Bind server_socket_information to the opened socket
    if(::bind(socket, (struct sockaddr*) &serverSocketInfo, sizeof(sockaddr)) < 0) {
        Log::error("Failed to bind socket");
        return false;
    }

    // Create a new thread to run the server in
    std::thread thread(&Server::run, this);
    thread.detach();
    
    return true;
}

bool Server::stop() {
    active = false;
    if(close(socket) < 0) {
        Log::error("Error in closing socket");
        return false;
    }
    return true;
}

void Server::run() {
    // Start listening
    listen(socket, queueSize);
    
    socklen_t addr_len = sizeof(clientSocketInfo);
    
    active = true;
    while(active) {
        // Accept incoming clients
        int clientSocket = accept(socket, (struct sockaddr*) &clientSocketInfo, &addr_len);
        if(!active) return;
        if(clientSocket < 0) {
            Log::error("Failed to accept client");
            continue;
        }
        
        std::thread thread(&Server::handle, this, clientSocket);
        thread.detach();
    }
}

bool Server::handle(int clientSocket) {
    // Create response
    Response response(clientSocket);
    
    // Receive request
    Request request;
    request.receive(clientSocket);
    
    // Callback
    bool success = callback(request, response);
    
    close(clientSocket); // TODO check for error
    return success;
}

