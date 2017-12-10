#include "error.hpp"
#include "util.hpp"

bool Error::respond(Request& request, Response& response, int code) {
    // Determine status belonging to this HTTP code
    std::string status;
    switch(code) {
        case 400: status = "400 Bad Request"; break;
        case 403: status = "403 Forbidden"; break;
        case 404: status = "404 Not Found"; break;
        case 405: status = "405 Method Not Allowed"; break;
            
        default:
            code = 500;
            status = "500 Internal Server Error"; break;
    }
    
    // Respond with corresponding error document
    response.setStatus(status);
    response.setHeader("Content-Type", Util::MIMEType("html"));
    
    response.sendStatus();
    response.sendHeaders();
    std::string path = "../web/error/" + std::to_string(code) + ".html";
    if(Util::fileExists(path))
        response.sendFile(path);
    else
        response.sendLine(status);
    
    return true;
}
