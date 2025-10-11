#include "RequestUidl.h"
#include "Server.h"

void RequestUidl::dispatch(Server& s) 
{
    s.handleRequest(*this);
}
