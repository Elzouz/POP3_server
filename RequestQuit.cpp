#include "RequestQuit.h"
#include "Server.h"

void RequestQuit::dispatch(Server& s) 
{
    s.handleRequest(*this);
}