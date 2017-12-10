#ifndef headers_hpp
#define headers_hpp

#include <string>
#include <unordered_map>
#include <regex>

class Response;

class Headers {
    
    static std::regex regexHeader;
    
    std::unordered_map<std::string, std::string> headers;
    
public:
    
    Headers();
    
    void set(std::string, std::string);
    void set(Headers*);
    std::string get(std::string);
    
    bool send(int);
    bool receive(int);
    
};

#endif
