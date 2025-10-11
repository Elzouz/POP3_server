#include "RequestList.h"
#include "Server.h"

void RequestList::dispatch(Server& s) 
{
    s.handleRequest(*this);
}