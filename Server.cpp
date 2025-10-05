#include <iostream>
#include "asio.hpp"
#include "Server.h"

namespace ip = asio::ip;

// On crée l'instance du Serveur avec un objet ASIO context qui sert à
// orchestrer tous les objets et événements gérés par ASIO (connexions réseau,
// timers, etc.)
Server::Server(asio::io_context& ioContext) :
	m_io{ioContext},
	// On crée la socket sur laquelle on écoutera
	m_acceptor{ioContext, ip::tcp::endpoint{ip::tcp::v4(), ECHO_PORT}}
{}

void Server::start()
{
	// On utilise une boucle infinie, chaque tour de boucle est l'interaction
	// complète avec un client jusqu'à sa déconnexion.
	// Le serveur quitte lorsqu'il crashe suite à une exception...
	for (;;)
	{
		m_currentState = State::WAITING_FOR_CONNECTION;

		// On initialise le flux de communication, qui contient une
		// socket non connectée pour le moment. On écoute sur le port
		// TCP/IP et lorsqu'un client se présente, la socket est alors
		// "branchée", et le flux m_stream permet de lire et écrire sur
		// la socket.
		// Pour d'autres manières de faire, par exemple, pour gérer
		// plusieurs clients simultanément, il y a quelques exemples
		// dans la documentation d'ASIO :
		// https://www.boost.org/doc/libs/1_57_0/doc/html/boost_asio/examples/cpp11_examples.html.
		m_stream = ip::tcp::iostream{};
		m_acceptor.accept(m_stream.socket());

		std::cerr << "New client connected!" << std::endl;

		m_stream << "Ready to repeat everything you say (one line at a time)! Type STOP to exit." << std::endl;

		// On passe à l'état suivant de la communication.
		m_currentState = State::TALKING_TO_USER;

		// On lit une ligne par une ligne tant qu'on peut.
		while (m_stream && m_currentState != State::QUITTING) {
			std::string line;
			std::getline(m_stream, line);
			// On a une seule façon de traiter les requêtes ici.
			// La méthode process est susceptible de déclencher une
			// transition vers un autre état du protocole.
			m_stream << process(line) << std::endl;
		}

		// Si l'état est QUITTING, cela signifie que l'on a quitté la
		// boucle volontairement, suite à une requête du client, et non
		// parce que le flux est devenu invalide (ce qui arrive si le
		// client s'est déconnecté par exemple).
		if (m_currentState == State::QUITTING) {
			m_stream << "Thank you for quitting politely instead of Ctrl+C-ing me like everyone else..." << std::endl;
		}

		// On ferme la connexion avec le client de façon polie (si on
		// détruit ou on réassigne l'objet m_stream, le client recevra
		// un TCP RESET à la place) et on signifie au système
		// d'exploitation qu'il peut libérer les ressources associées
		// dans les structures de données réseau.
		//
		// Il faut toujours vérifier si le flux est dans un état valide,
		// il se peut que la connexion soit déjà fermée si on a soi-même
		// reçu un TCP RESET.
		if (m_stream) {
			m_stream.close();
		}
	}
}

// Fonction de traitement de n'importe quel message du client, très basique.
std::string Server::process(const std::string& req)
{
	if (req.substr(0,4) == "STOP") {
		m_currentState = State::QUITTING;
		return "bye!";
	} else {
		return req;
	}
}
