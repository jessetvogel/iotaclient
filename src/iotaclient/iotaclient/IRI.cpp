#include "IRI.hpp"
#include "util.hpp"
#include "error.hpp"
#include "client.hpp"
#include "log.hpp"

#include <iostream>

#define MAX_DATA_SIZE (4096)

std::string IRI::nodeHost("node01.iotatoken.nl");
int IRI::nodePort = 14265;

std::regex IRI::regexIRI("^\\/iri$");

bool IRI::handle(Request& request, Response& response) {
    // Check if we are to handle this request
    std::string requestPath = request.getRequestURI();
    std::cmatch cm;
    
    // General resources
    if(std::regex_match(requestPath.c_str(), cm, regexIRI)) {
        // Require POST method
        if(request.getMethod().compare("POST") != 0) return Error::respond(request, response, 405);
        
        // Get content length
        std::string _contentLength = request.getHeaders()->get("Content-Length");
        if(!Util::isInteger(_contentLength)) return Error::respond(request, response, 400);
        int contentLength = stoi(_contentLength);
        if(contentLength > MAX_DATA_SIZE) return Error::respond(request, response, 413);
        
        // Get data
        char command[contentLength];
        int commandLength = request.getData(command, contentLength);
        
        // Make request to IRI
        Client client(nodeHost, nodePort);
        Request* req = client.createRequest();
        if(req == NULL) return Error::respond(request, response, 500);
        
        req->setMethod("POST");
        req->setRequestURI("/");
        req->setHeader("Host", nodeHost);
        req->setHeader("X-IOTA-API-Version", request.getHeaders()->get("X-IOTA-API-Version"));
        req->setHeader("Content-Type", request.getHeaders()->get("Content-Type"));
        req->setHeader("Content-Length", request.getHeaders()->get("Content-Length"));
        
        req->sendRequestLine();
        req->sendHeaders();
        req->sendData(command, commandLength);
        
        // Get response back from server
        Response* res = client.getResponse();
        _contentLength = res->getHeaders()->get("Content-Length");
        if(!Util::isInteger(_contentLength)) return Error::respond(request, response, 500);
        contentLength = stoi(_contentLength);
        
        char data[contentLength];
        int dataLength = res->getData(data, contentLength);
        
        // Send response back to our client
        response.setStatus(res->getStatus());
        response.setHeader(res->getHeaders());
        response.sendStatus();
        response.sendHeaders();
        response.sendData(data, dataLength);
        
        return true;
    }
    
    return false;
}
