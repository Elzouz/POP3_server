#include "RequestRst.h"
#include "Server.h"

void RequestRst::dispatch(Server& s) 
{
    s.handleRequest(*this);
}
