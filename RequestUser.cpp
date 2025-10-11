#include "RequestUser.h"
#include "Server.h"

void RequestUser::dispatch(Server& s) 
{
    s.handleRequest(*this);
}
