#include "log.hpp"
#include <time.h>
#include <iostream>

#define COLOR_DEFAULT "\e[39m"
#define COLOR_WHITE "\e[97m"
#define COLOR_BLACK "\e[30m"
#define COLOR_RED "\e[31m"
#define COLOR_YELLOW "\e[33m"
#define COLOR_GREEN "\e[32m"
#define COLOR_CYAN "\e[36m"
#define COLOR_BLUE "\e[34m"
#define COLOR_MAGENTA "\e[35m"

std::string Log::tag() {
    time_t rawTime;
    struct tm * timeInfo;
    time(&rawTime);
    timeInfo = localtime(&rawTime);
    char buffer[64];
    return std::string(buffer, strftime(buffer, 64, "[ %H:%M:%S ] ", timeInfo));
}

void Log::print(std::string message) {
    std::cout << tag() << message << std::endl;
}

void Log::error(std::string message) {
    std::cerr << tag() << COLOR_RED << message << COLOR_DEFAULT << std::endl;
}

