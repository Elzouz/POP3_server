#include <iostream>
#include<sstream>
#include "asio.hpp"
#include "Request.h"

std::string Request::parseRequest(std::istream& req){
    std::string line; 
    std::getline(req, line); // Lit une ligne complète du flux
    std::istringstream is{line}; // Crée un sous-flux pour analyser la ligne
    std::string cmd; 
    is >> cmd; // Lit le premier mot de la ligne et détermine la commande
    if (cmd == "USER") command = Command::USER;
    else if (cmd == "PASS") command = Command::PASS;
    else if (cmd == "STAT") command = Command::STAT;
    else if (cmd == "LIST") command = Command::LIST;
    else if (cmd == "RETR") command = Command::RETR;
    else if (cmd == "DELE") command = Command::DELE;
    else command = Command::INVALID;
    return line;
}


Request::Command Request::getCommand(){
    return command;
}
