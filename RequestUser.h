#ifndef REQUEST_USER_H
#define REQUEST_USER_H

#include "Request.h"

class Server;
class RequestUser : public Request
{
private:

public : 
    using Request::Request;
    virtual void dispatch(Server& s) override; //polymorphisme dynamique
};

#endif