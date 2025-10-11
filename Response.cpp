#include "Response.h"
#include "Request.h"

Response::Response(Request& request) :
    req{request}
{
    setResponse();
}

std::string Response::getResp() const
{
    return resp;
}

void Response::setResponse()
{
    Request::Command cmd = req.getCommand();
    if (cmd == Request::Command::INVALID) resp = "-ERR " + req.getDescription();
    else resp = "+OK " + req.getDescription();
}

std::ostream& operator<<(std::ostream& os, const Response& r)
{
    os << r.getResp();
  return os;
}