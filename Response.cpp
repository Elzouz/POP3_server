#include <iostream>
#include "Response.h"
#include "Request.h"

std::string Response::getMessage() const{
    return message;
}

void Response::setMessage(Request& req){
   if(req.getCommand() == Request::Command::INVALID){
        message = "-ERR\r\n";
   }
   else{
        message = "+OK\r\n";
   }
}

std::ostream& operator<<(std::ostream& os, const Response& r){
    os << r.getMessage() << "\n";
    return os;
}

