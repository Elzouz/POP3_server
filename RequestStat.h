#ifndef REQUEST_STAT_H
#define REQUEST_STAT_H

#include "Request.h"

class Server;
class RequestStat : public Request
{
private:

public : 
    using Request::Request;
    virtual void dispatch(Server& s) override; //polymorphisme dynamique
};

#endif