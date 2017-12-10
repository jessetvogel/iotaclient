#ifndef IRI_hpp
#define IRI_hpp

#include <regex>

#include "request.hpp"
#include "response.hpp"

class IRI {
    
    static std::string nodeHost;
    static int nodePort;
    
    static std::regex regexIRI;
    
public:
    
    static bool handle(Request&, Response&);
    
};

#endif
