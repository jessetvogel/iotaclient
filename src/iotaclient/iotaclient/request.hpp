#ifndef request_hpp
#define request_hpp

// https://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html

#include <string>
#include <regex>
#include "headers.hpp"

#define DEFAULT_METHOD ("GET")
#define DEFAULT_REQUEST_URI ("/")
#define DEFAULT_HTTP_VERSION ("HTTP/1.1")

class Request {

    static std::regex regexRequestLine;
    static std::regex regexRequestURI;
    
public: int socket;
    
    std::string method;
    std::string requestURI;
    std::string requestPath;
    std::string requestQuery;
    std::string httpVersion;
    
    Headers headers;
        
public:
    
    Request(int socket) : socket(socket), method(DEFAULT_METHOD), requestURI(DEFAULT_REQUEST_URI), httpVersion(DEFAULT_HTTP_VERSION) {}
    Request() : Request(-1) {}

    bool receive(int);
    
    inline std::string getMethod() { return method; }
    inline std::string getRequestURI() { return requestURI; }
    inline std::string getRequestPath() { return requestPath; }
    inline std::string getRequestQuery() { return requestQuery; }
    inline std::string getHTTPVersion() { return httpVersion; }
    inline Headers* getHeaders() { return &headers; }
    int getData(void*, int);
    
    inline void setMethod(std::string method) { this->method = method; }
    inline void setRequestURI(std::string requestURI) { this->requestURI = requestURI; }
    inline void setHeader(std::string field, std::string value) { headers.set(field, value); }
    inline void setHeader(Headers* other) { headers.set(other); }
    
    bool sendRequestLine();
    bool sendHeaders();
    bool sendLine();
    bool sendData(void*, int);
    
};

#endif
