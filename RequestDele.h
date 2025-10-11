#ifndef REQUESTDELE_H
#define REQUESTDELE_H

#include "Request.h"

class Server;
class RequestDele : public Request
{
private:

public : 
    using Request::Request;
    virtual void dispatch(Server& s) override; //polymorphisme dynamique
};

#endif