#ifndef REQUEST_TOP_H
#define REQUEST_TOP_H

#include "Request.h"

class Server;
class RequestTop : public Request
{
private :
    
public :
    using Request::Request;

    virtual void dispatch(Server& s) override; //polymorphisme dynamique
};

#endif