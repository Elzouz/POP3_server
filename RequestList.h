#ifndef REQUEST_LIST_H
#define REQUEST_LIST_H

#include "Request.h"

class Server;
class RequestList : public Request
{
private:

public : 
    using Request::Request;
    virtual void dispatch(Server& s) override; //polymorphisme dynamique
};

#endif