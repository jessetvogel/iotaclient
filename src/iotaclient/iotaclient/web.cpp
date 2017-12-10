#include "web.hpp"
#include "util.hpp"
#include "error.hpp"

std::regex Web::regexPages("^\\/(settings|help|debug)?$");
std::regex Web::regexGeneralResource("^\\/((?:js|css|fonts|img)\\/.*|favicon\\.ico)$");
std::regex Web::regexInstrumentResource("^\\/instrument\\/(\\w+\\/.+)$");
std::regex Web::regexExtension("^.*?(?:\\.(\\w+))?$");

bool Web::handle(Request& request, Response& response) {
    // Check if we are to handle this request
    std::string requestPath = request.getRequestURI();
    std::cmatch cm;
    
    // General resources
    if(std::regex_match(requestPath.c_str(), cm, regexGeneralResource)) {
        std::string path = "../web/" + std::string(cm[1]);
        return sendFile(request, response, path);
    }
    
    // Pages
    if(std::regex_match(requestPath.c_str(), cm, regexPages)) {
        std::string page = cm[1];
        if(page.compare("") == 0)            return Web::sendFile(request, response, "../web/index.html");
//        if(page.compare("settings") == 0)    return Web::sendFile(request, response, "../web/pages/settings.html");
        
        return Error::respond(request, response, 404);
    }
    
    return false;
}

bool Web::sendFile(Request& request, Response& response, std::string path) {
    std::cmatch cm;
    if(!std::regex_match(path.c_str(), cm, regexExtension)) return false;
    
    // Check if the file exists
    if(!Util::fileExists(path))
        return Error::respond(request, response, 404);
    
    response.setHeader("Content-Type", Util::MIMEType(cm[1]));
    
    response.sendStatus();
    response.sendHeaders();
    response.sendFile(path);
    
    return true;
}