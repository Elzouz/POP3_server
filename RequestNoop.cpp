#include "RequestNoop.h"
#include "Server.h"

void RequestNoop::dispatch(Server& s) 
{
    s.handleRequest(*this);
}
