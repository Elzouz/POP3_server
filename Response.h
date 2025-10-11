#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include "Request.h"

class Response
{
private:
    std::string resp;
    Request& req;
public:
    Response(Request& request);

    void setResponse();
    std::string getResp() const;
};

std::ostream& operator<<(std::ostream& os, const Response& r);

#endif