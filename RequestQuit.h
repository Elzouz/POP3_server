#ifndef REQUEST_QUIT_H
#define REQUEST_QUIT_H

#include "Request.h"

class Server;
class RequestQuit : public Request
{
private :
    
public :
    using Request::Request; // Cas particulier de Quit pas de override 

    virtual void dispatch(Server& s) override; //polymorphisme dynamique
};

#endif