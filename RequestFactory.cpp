#include <iostream>
#include <sstream>
#include <vector>

#include "Server.h"
#include "Request.h"
#include "RequestUser.h"
#include "RequestPass.h"
#include "RequestQuit.h"
#include "RequestDele.h"
#include "RequestStat.h"
#include "RequestList.h"
#include "RequestRetr.h"
#include "RequestNoop.h"
#include "RequestRst.h"
#include "RequestUidl.h"
#include "RequestTop.h"

#include "RequestFactory.h"

Request* RequestFactory::parseRequest(std::istream& stream, Server::State state)
{
    Request* req;
    std::string line;
    std::getline(stream, line);
    // Enlever un éventuel '\r' terminal (CRLF des clients POP3)
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }
    std::istringstream is{line};

    std::string first;
    is >> first;

    std::vector<std::string> arguments;
    std::string arg;
    while (is >> arg) {
        arguments.push_back(arg);
    }

    if (state == Server::State::WAITING_FOR_CONNECTION) {
        if (first == "QUIT" ) {
            Request::Command command = Request::Command::QUIT;
            req = new RequestQuit(command, arguments);
        }
        
        else if (first == "USER" ) {
            Request::Command command = Request::Command::USER;
            if (arguments.size() != 1){
                command = Request::Command::INVALID;
                req = new Request(command, arguments, "USER command requires exactly one argument");
                return req;
            }
            req = new RequestUser(command, arguments);
        }
        else {
            Request::Command command = Request::Command::INVALID;
            req = new Request(command, arguments, "Only USER and QUIT commands are allowed in this state");
        }
    }

    else if (state == Server::State::WAITING_FOR_PASSWORD) {
        if (first == "QUIT" ) {
            Request::Command command = Request::Command::QUIT;
            req = new RequestQuit(command, arguments);
        }
        
        else if (first == "PASS" ) {
            Request::Command command = Request::Command::PASS;
            if (arguments.size() != 1){
                command = Request::Command::INVALID;
                req = new Request(command, arguments, "PASS command requires exactly one argument");
                return req;
            }
            req = new RequestPass(command, arguments);
        }
        else {
            Request::Command command = Request::Command::INVALID;
            req = new Request(command, arguments, "Only PASS and QUIT commands are allowed in this state");
        }
    }
    
    else if (state == Server::State::TALKING_TO_USER) {
        if (first == "QUIT" ) {
            Request::Command command = Request::Command::QUIT;
            req = new RequestQuit(command, arguments);
        }
        
        else if (first == "STAT" ) {
            Request::Command command = Request::Command::STAT;
            if(arguments.size() == 0) {
                req = new RequestStat(command, arguments);
            }
            else {
                command = Request::Command::INVALID;
                req = new Request(command, arguments, "STAT command takes no arguments");
            }
        }

        else if (first == "LIST" ) {
            Request::Command command = Request::Command::LIST;
            if(arguments.size() == 0) {
                req = new RequestList(command, arguments);
            }
            else if(arguments.size() == 1){
                req = new RequestList(command, arguments);
            }
            else {
                command = Request::Command::INVALID;
                req = new Request(command, arguments, "LIST command takes zero or one argument");
            }
        }

        else if (first == "RETR" ) {
            Request::Command command = Request::Command::RETR;
            if(arguments.size() != 1){
                command = Request::Command::INVALID;
                req = new Request(command, arguments, "RETR command requires exactly one argument");
                return req;
            }
            std::vector<std::string> messageToRetr = {arguments[0]};
            req = new RequestRetr(command, messageToRetr);
        }

        else if (first == "DELE" ) {
            Request::Command command = Request::Command::DELE;
            if(arguments.size() != 1){
                command = Request::Command::INVALID;
                req = new Request(command, arguments, "DELE command requires exactly one argument");
                return req;
            }
            std::vector<std::string> messageToDel = {arguments[0]};
            std::string mes = "Message " + arguments[0] + " deleted";
            req = new RequestDele(command, messageToDel, mes);
        }
        
        else if (first == "NOOP" ) {
            Request::Command command = Request::Command::NOOP;
            if(arguments.size() != 0){
                command = Request::Command::INVALID;
                req = new Request(command, arguments, "NOOP command takes no arguments");
                return req;
            }
            req = new RequestNoop(command, arguments);
        }

        else if (first == "RSET" ) {
            Request::Command command = Request::Command::RSET;
            if(arguments.size() != 0){
                command = Request::Command::INVALID;
                req = new Request(command, arguments, "RSET command takes no arguments");
                return req;
            }
            req = new RequestRst(command, arguments);
        }

        else if (first == "TOP" ) {
            Request::Command command = Request::Command::TOP;
            if(arguments.size() != 2){
                command = Request::Command::INVALID;
                req = new Request(command, arguments, "TOP command requires exactly two arguments");
                return req;
            }
            req = new RequestTop(command, arguments);
        }

        else if (first == "UIDL" ) {
            Request::Command command = Request::Command::UIDL;
            if(arguments.size() == 0) {
                req = new RequestUidl(command, arguments);
                
            }
            else if(arguments.size() == 1){
                req = new RequestUidl(command, arguments);
            }
            else {
                command = Request::Command::INVALID;
                req = new Request(command, arguments, "UIDL command takes zero or one argument");
            }
        }

        else {
            Request::Command command = Request::Command::INVALID;
            req = new Request(command, arguments);
        }
    }
    
    return req;
}