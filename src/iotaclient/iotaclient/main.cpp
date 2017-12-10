#include <string>

#include "server.hpp"
#include "web.hpp"
#include "IRI.hpp"
#include "error.hpp"
#include "log.hpp"
#include <iostream>

bool callback(Request& request, Response& response) {
    // Use relevant handler
    if(Web::handle(request, response)) return true;
    if(IRI::handle(request, response)) return true;

    // Show some request information
    return Error::respond(request, response, 400);
}

int main(int argc, const char * argv[]) {
    // Create and start server
    Server server(8080, &callback, 10);
    if(!server.start()) return 0;
    Log::print("Started on port " + std::to_string(server.getPort()));
    
    // Wait for input
    std::string line;
    while(std::getline(std::cin, line)) {
        // Check for exit command
        if(line.compare("exit") == 0) break;
    }
    
    // Stop server
    server.stop();
    
    return 0;
}
