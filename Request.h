#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <vector>

#include "Server.h"

class Request{
public : 
    enum class Command{
            USER,
            PASS,
            STAT,
            LIST,
            RETR,
            DELE,
            INVALID
        };

private:
    Command command;

public:
    std::string parseRequest(std::istream& req);
    Command getCommand();

    void dispatch(Server); // A quoi ça sert ??
};

#endif