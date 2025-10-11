#ifndef REQUEST_FACTORY_H
#define REQUEST_FACTORY_H

#include <string>

#include "Request.h"

class Server;

class RequestFactory
{
private :

public :
    Request* parseRequest(std::istream& stream, Server::State state);
};


#endif