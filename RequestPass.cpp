#include "RequestPass.h"
#include "Server.h"

void RequestPass::dispatch(Server& s) 
{
    s.handleRequest(*this);
}