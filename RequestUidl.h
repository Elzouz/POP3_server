#ifndef REQUEST_UIDL_H
#define REQUEST_UIDL_H

#include "Request.h"

class Server;
class RequestUidl : public Request
{
private :
    
public :
    using Request::Request;

    virtual void dispatch(Server& s) override; //polymorphisme dynamique
};

#endif