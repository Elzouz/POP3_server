#include "RequestRetr.h"
#include "Server.h"

void RequestRetr::dispatch(Server& s) 
{
    s.handleRequest(*this);
}
