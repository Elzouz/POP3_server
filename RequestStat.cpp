#include "RequestStat.h"
#include "Server.h"

void RequestStat::dispatch(Server& s) 
{
    s.handleRequest(*this);
}
