#ifndef socket_hpp
#define socket_hpp

#include <string>

class Socket {

public:
    
    static bool write(int, std::string);
    static bool writeLine(int);
    static bool writeLine(int, std::string);
    static bool writeFile(int, std::string);
    static bool writeData(int, void*, int);
    
    static int read(int, void*, int);
    static std::string readLine(int);
};

#endif
