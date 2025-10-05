#ifndef RESPONSE_H
#define RESPONSE_H  
#include <iostream>
#include <string>
#include "Request.h"

class Response{
private:
    std::string message;
public:
    std::string getMessage() const;
    void setMessage(Request& req);
};
  
std::ostream& operator<<(std::ostream& os, const Response& r);

#endif