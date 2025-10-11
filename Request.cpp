#include <iostream>
#include <sstream>
#include <vector>

#include "Request.h"
#include "Server.h"

Request::Request(Command c, std::vector<std::string> args, std::string desc) :
    command{c},
    arguments{args},
    description{desc}
{
}

Request::Command Request::getCommand()
{
    return command;
}


std::vector<std::string> Request::getArguments()
{
    return arguments;
}

std::string Request::getDescription()
{
    return description;
}

void Request::dispatch(Server& s) 
{
    s.handleRequest(*this);
}