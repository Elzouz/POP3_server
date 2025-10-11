#ifndef REQUEST_RETR_H
#define REQUEST_RETR_H

#include "Request.h"

class Server;
class RequestRetr : public Request
{
private:

public : 
    using Request::Request;
    virtual void dispatch(Server& s) override; //polymorphisme dynamique
};

#endif