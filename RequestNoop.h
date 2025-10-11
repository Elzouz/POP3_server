#ifndef REQUEST_NOOP_H
#define REQUEST_NOOP_H

#include "Request.h"

class Server;
class RequestNoop : public Request
{
private:

public : 
    using Request::Request;
    virtual void dispatch(Server& s) override; //polymorphisme dynamique
};

#endif