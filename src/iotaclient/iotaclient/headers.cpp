#include "headers.hpp"
#include "socket.hpp"

#define REGEX_HEADER_KEY "\\S+"
#define REGEX_HEADER_VALUE ".+"

std::regex Headers::regexHeader("^(" REGEX_HEADER_KEY "): (" REGEX_HEADER_VALUE ")$");

Headers::Headers() {
    // Set defaults
    set("Content-Type", "text/html");
    set("Date", "now");
}

void Headers::set(std::string field, std::string value) {
    headers[field] = value;
}

void Headers::set(Headers* other) {
    for(auto it = other->headers.begin();it != other->headers.end(); ++it)
        headers[it->first] = it->second;
}

std::string Headers::get(std::string field) {
    auto position = headers.find(field);
    if(position == headers.end()) return "";
    return position->second;
}

bool Headers::send(int socket) {
    bool success = true;
    for(auto it = headers.begin();it != headers.end(); ++it) {
        success &=
            Socket::write(socket, it->first) &&
            Socket::write(socket, ": ") &&
            Socket::writeLine(socket, it->second);
    }
    success &= Socket::writeLine(socket);
    return success;
}

bool Headers::receive(int socket) {
    headers.clear();
    std::cmatch cm;
    std::string line;
    while(true) {
        line = Socket::readLine(socket);
        if(line.length() == 0) break;
        if(!std::regex_search(line.c_str(), cm, regexHeader)) return false;
        set(cm[1], cm[2]);
    }
    return true;
}
