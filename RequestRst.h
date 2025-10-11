#ifndef REQUEST_RST_H
#define REQUEST_RST_H

#include "Request.h"

class Server;
class RequestRst : public Request
{
private :
    
public :
    using Request::Request;

    virtual void dispatch(Server& s) override; //polymorphisme dynamique
};

#endif