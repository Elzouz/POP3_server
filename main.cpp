#include <iostream>
#include <vector>
#include <string>

#include "asio.hpp"
#include "Server.h"
#include "Message.h"
#include "BoiteAuxLettres.h"

int main()
{
    //using namespace centralesupelec::tp2;

    // On crée un contexte par défaut que l'on passe au Serveur. Lorsque l'on
    // veut faire des opérations asynchrones (pour gérer des timeouts, ou faire
    // du multithreading, ou parler avec plusieurs clients à la fois), il faut
    // aussi lancer la file d'événement et les processeurs du contexte avec
    // io_context::run().
    asio::io_context io_context;

    // Création d'une boîte aux lettres fictive et ajout de quelques messages
	BoiteAuxLettres boiteAuxLettres;
	Message m1 {"Sujet1", "Expediteur1", std::chrono::system_clock::now(), "Contenu du message 1 plus long que les autres"};
    Message m2 {"Sujet2", "Expediteur2", std::chrono::system_clock::now(), "Contenu du message 2"};
    Message m3 {"Sujet3", "Expediteur3", std::chrono::system_clock::now(), "Contenu du message 3 de plusieurs lignes\r\nLigne 2\r\nLigne3\r\n."};

    boiteAuxLettres.AddMessage(m1);
	boiteAuxLettres.AddMessage(m2);
	boiteAuxLettres.AddMessage(m3);

    Server pop3{io_context, boiteAuxLettres};

    // Cette méthode ne quitte pas (sauf quand elle finit par le faire).
    pop3.start();
}
