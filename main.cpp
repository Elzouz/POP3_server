#include <iostream>

#include "asio.hpp"
#include "Server.h"

int main()
{
    // On crée un contexte par défaut que l'on passe au Serveur. Lorsque l'on
    // veut faire des opérations asynchrones (pour gérer des timeouts, ou faire
    // du multithreading, ou parler avec plusieurs clients à la fois), il faut
    // aussi lancer la file d'événement et les processeurs du contexte avec
    // io_context::run().
    asio::io_context io_context;
    Server pop3{io_context};

    // Cette méthode ne quitte pas (sauf quand elle finit par le faire).
    pop3.start();
}
