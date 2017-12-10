#ifndef server_hpp
#define server_hpp

#include <thread>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "request.hpp"
#include "response.hpp"

class Server {

    int port;
    int queueSize;
    bool (*callback)(Request&, Response&);
    
    int socket;
    struct sockaddr_in serverSocketInfo;
    struct sockaddr_in clientSocketInfo;
    
    bool active;
    void run();
    bool handle(int);
    
public:
    
    Server(int port, bool (*callback)(Request&, Response&), int queueSize) : port(port), callback(callback), queueSize(queueSize) {}
    inline ~Server() { if(active) stop(); }
    
    bool start();
    bool stop();
    
    inline int getPort() { return port; }
    
};

#endif
