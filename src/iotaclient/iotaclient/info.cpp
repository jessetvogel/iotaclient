#include "info.hpp"
#include "util.hpp"
#include "error.hpp"

bool Info::writeJSON(Request& request, Response& response, std::string json) {
    response.setHeader("Content-Type", Util::MIMEType("json"));
    response.setHeader("Content-Length", std::to_string(json.length()));
    
    response.sendStatus();
    response.sendHeaders();
    response.sendLine(json);
    
    return true;
}
