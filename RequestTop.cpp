#include "RequestTop.h"
#include "Server.h"

void RequestTop::dispatch(Server& s) 
{
    s.handleRequest(*this);
}
