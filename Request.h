#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <vector>

// Cas "STOP"
// Cas \r\n gérer le \r ne pas le prendre dans le message 
// Cas . fin de message sur plusieurs lignes (.. si déjà .)
// >> fin d'un message plusieurs lignes : \r\n.\r\n

class Server;

class Request
{
public:
    enum class Command
    {
        QUIT,
        USER,
        PASS,
        STAT,
        LIST,
        RETR,
        DELE,
        NOOP,
        RSET,
        TOP,
        UIDL,
        INVALID
    };
    
private:
    Command command;
    std::vector<std::string> arguments;
    std::string description;

public :
    Request(Command command, std::vector<std::string> arguments = {}, std::string description = "");
    virtual ~Request() = default;
    Command getCommand();
    std::vector<std::string> getArguments();
    std::string getDescription();

    virtual void dispatch(Server& s); //polymorphisme dynamique
};


#endif