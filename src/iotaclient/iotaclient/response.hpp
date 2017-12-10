#ifndef response_hpp
#define response_hpp

// https://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html

#include <string>
#include <regex>
#include "headers.hpp"

#define DEFAULT_HTTP_VERSION ("HTTP/1.1")
#define DEFAULT_STATUS ("")

class Response {

    static std::regex regexStatus;

    int socket;

    std::string httpVersion;
    std::string status;
    Headers headers;
    
public:
    
    Response(int socket) : socket(socket), httpVersion(DEFAULT_HTTP_VERSION), status(DEFAULT_STATUS) {}
    Response() : Response(-1) {}
    
    bool receive(int);
    
    inline std::string getHTTPVersion() { return httpVersion; }
    inline std::string getStatus() { return status; }
    inline Headers* getHeaders() { return &headers; }
    int getData(void*, int);
    
    inline void setStatus(std::string status) { this->status = status; }
    inline void setHeader(std::string field, std::string value) { headers.set(field, value); }
    inline void setHeader(Headers* other) { headers.set(other); }
    
    bool sendStatus();
    bool sendHeaders();
    bool sendLine(std::string);
    bool sendFile(std::string);
    bool sendData(void*, int);
    
};

#endif
