#ifndef REQUEST_PASS_H
#define REQUEST_PASS_H

#include "Request.h"

class Server;
class RequestPass : public Request
{
private:

public : 
    using Request::Request;
    virtual void dispatch(Server& s) override; //polymorphisme dynamique
};

#endif