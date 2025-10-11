#include "RequestDele.h"
#include "Server.h"

void RequestDele::dispatch(Server& s) 
{
    s.handleRequest(*this);
}
