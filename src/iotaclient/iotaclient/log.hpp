#ifndef log_hpp
#define log_hpp

#include <string>

class Log {
    
    static std::string tag();
    
public:
    
    static void print(std::string);
    static void error(std::string);
    
};

#endif
