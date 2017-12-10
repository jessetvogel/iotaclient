#ifndef client_hpp
#define client_hpp

#include <string>
#include "request.hpp"
#include "response.hpp"

class Client {

    const std::string host;
    const int port;

    int socket;
    
    Request* request;
    Response* response;
        
public:
    
    Client(std::string host, int port) : host(host), port(port), socket(-1), request(NULL), response(NULL) {};
    ~Client();
    
    Request* createRequest();
    Response* getResponse();
    
};

#endif
