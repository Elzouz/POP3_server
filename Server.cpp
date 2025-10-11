#include <iostream>

#include "asio.hpp"

#include "Server.h"
#include "Message.h"
#include "BoiteAuxLettres.h"

#include "Request.h"
#include "RequestQuit.h"
#include "RequestDele.h"
#include "RequestUser.h"
#include "RequestPass.h"
#include "RequestStat.h"
#include "RequestList.h"
#include "RequestRetr.h"
#include "RequestNoop.h"
#include "RequestRst.h"
#include "RequestUidl.h"
#include "RequestTop.h"

#include "RequestFactory.h"

#include "Response.h"

//namespace centralesupelec::tp2
//{
namespace ip = asio::ip;


// On crée l'instance du Serveur avec un objet ASIO context qui sert à
// orchestrer tous les objets et événements gérés par ASIO (connexions réseau,
// timers, etc.)
Server::Server(asio::io_context& ioContext, BoiteAuxLettres b) :
	m_io{ioContext},
	boiteAuxLettres{b},
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

		m_stream << "+OK POP3 server ready" << std::endl;

		// On passe à l'état suivant de la communication.
		// m_currentState = State::TALKING_TO_USER;

		// On lit une ligne par une ligne tant qu'on peut.
		while (m_stream && m_currentState != State::QUITTING) {

			RequestFactory * reqFact = new RequestFactory();
			Request* req = reqFact->parseRequest(m_stream, m_currentState);

			if (req) {
				req->dispatch(*this);
				delete req;      // Requête créée dans requestFactory avec parseRequest
				req = nullptr;
			}

			delete reqFact;
			reqFact = nullptr;

		
			// On a une seule façon de traiter les requêtes ici.
			// La méthode process est susceptible de déclencher une
			// transition vers un autre état du protocole.
			//process(line);
			/* VERIF MESSAGES
			std::vector<Message> messages = boiteAuxLettres.sgetAllMessages();
			for(Message& msg : messages){
				std::cout << msg << std::endl;
			}*/
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

// Fonction gérant les erreurs d'une requête invalide
void Server::handleRequest(Request& req)
{
	Response response = Response(req);
	std::cout << response << std::endl;

}

void Server::handleRequest(RequestUser& req)
{
	// Ici on accepte tous les noms d'utilisateur (la validité de la requête est vérifiée dans RequestFactory)
	std::cout << "+OK User accepted" << std::endl;
	m_currentState = State::WAITING_FOR_PASSWORD;
}

void Server::handleRequest(RequestPass& req)
{
	// Ici on accepte tous les mots de passe (la validité de la requête est vérifiée dans RequestFactory)
	std::cout << "+OK Password accepted" << std::endl;
	m_currentState = State::TALKING_TO_USER;
}

void Server::handleRequest(RequestQuit& req)
{
	boiteAuxLettres.deleteMessage();
	Response response = Response(req);
	std::cout << response << std::endl;
	m_currentState = State::QUITTING;
}

void Server::handleRequest(RequestDele& req)
{
	// Récupérer l'ID du message à supprimer depuis la requête
	std::vector<std::string> args = req.getArguments();
	int messageId = std::stoi(args[0]);
	Message * msgToDelete = boiteAuxLettres.getMessageById(messageId);
	if (msgToDelete != nullptr && !msgToDelete->isToBeDeleted()) {
		msgToDelete->markToBeDeleted();

		// Le message a été marqué pour suppression, on envoie une réponse positive
		Response response = Response(req);
		std::cout << response << std::endl;
	}
	// Si le message n'existe pas, on envoie une réponse d'erreur bien que la requête soit considérée comme valide
	else {
		std::cout << "-ERR. Message with ID " << messageId << " not found." << std::endl;
	}
}

void Server::handleRequest(RequestList& req)
{
	const std::vector<Message>& messages = boiteAuxLettres.getAllMessages();
	if(req.getArguments().size() == 0){	

		std::pair<int, int> stats = boiteAuxLettres.countSize();
		std::cout << "+OK " << stats.first << " (" << stats.second << " octets)" << std::endl;
		for(const Message& msg : messages){
			if(!msg.isToBeDeleted()) {
				std::cout << msg.getId() << " " << msg.getSize() << std::endl;
			}
		}
		std::cout << "." << std::endl;
	}
	else if(req.getArguments().size() == 1){
		int requestedId = std::stoi(req.getArguments()[0]);
		Message* msg = boiteAuxLettres.getMessageById(requestedId);
		if(msg != nullptr && !msg->isToBeDeleted()){
			std::cout << "+OK " << requestedId << " " << msg->getSize() << std::endl;
		}
		else {
			std::cout << "-ERR. No such message" << std::endl;
		}
	}
}

void Server::handleRequest(RequestStat& req)
{
	if(req.getArguments().size() == 0){	
		std::pair<int, int> stats = boiteAuxLettres.countSize();
		std::cout << "+OK " << stats.first << " "<< stats.second << std::endl;
	}
	else {
		Response response = Response(req);
		std::cout << response << std::endl;
	}
}

void Server::handleRequest(RequestRetr& req)
{
	if(req.getArguments().size() == 1) {
		int requestedId = std::stoi(req.getArguments()[0]);
		Message * msg = boiteAuxLettres.getMessageById(requestedId);
		if(msg != nullptr && !msg->isToBeDeleted()){
			std::cout << "+OK " << msg->getSize() << " octets" << std::endl;
			std::cout << *msg << std::endl;
		}
		else {
			std::cout << "-ERR. No such message" << std::endl;
		}
	}
}

void Server::handleRequest(RequestNoop& req)
{
	Response response = Response(req);
	std::cout << response << std::endl;
}

void Server::handleRequest(RequestRst& req)
{
	std::vector<Message>& messages = boiteAuxLettres.getAllMessages();
	for(Message& msg : messages){
		if(msg.isToBeDeleted()) {
			msg.doNotDelete();
		}
	}
	std::pair<int, int> stats = boiteAuxLettres.countSize();
	std::cout << "+OK, maildrop has " << stats.first << " messages (" << stats.second << " octets)" << std::endl;
}

void Server::handleRequest(RequestTop& req)
{
	if(req.getArguments().size() == 2) {
		int requestedId = std::stoi(req.getArguments()[0]);
		int numLines = std::stoi(req.getArguments()[1]);
		Message * msg = boiteAuxLettres.getMessageById(requestedId);
		if(msg != nullptr && !msg->isToBeDeleted()){
			if(numLines < 0) {
				std::cout << "-ERR number of lines must be positive" << std::endl;
				return;
			}
			std::cout << "+OK" << std::endl;
			std::cout << "Subject: " << msg->getSubject() << std::endl;
			std::cout << "From: " << msg->getExpeditor() << std::endl;
			std::cout << "Date: " << msg->affichageHorodatage(msg->getDate()) << std::endl;
			std::cout << std::endl;

			// Afficher les premières lignes du contenu
			std::istringstream contentStream(msg->getContent());
			std::string line;
			int lineCount = 0;
			while (std::getline(contentStream, line) && lineCount < numLines) {
				std::cout << line << std::endl;
				lineCount++;
			}
			std::cout << "." << std::endl; // Fin de la réponse
		}
		else {
			std::cout << "-ERR. No such message" << std::endl;
		}
	}
}

void Server::handleRequest(RequestUidl& req)
{
	if (req.getArguments().size() == 0) {
		std::vector<Message>& messages = boiteAuxLettres.getAllMessages();
		std::cout << "+OK" << std::endl;
		for(Message& msg : messages){
			if(!msg.isToBeDeleted()){
				std::cout << msg.getId() << " " << msg.getUidl() << msg.getUidl() << std::endl;
			}
		}
		std::cout << "." << std::endl;
	}
	else if (req.getArguments().size() == 1) {
		int requestedId = std::stoi(req.getArguments()[0]);
		Message * msg = boiteAuxLettres.getMessageById(requestedId);
		if(msg != nullptr && !msg->isToBeDeleted()){
			std::cout << "+OK " << msg->getId() << " " << msg->getUidl() << std::endl;
		}
		else {
			std::cout << "-ERR. No such message" << std::endl;
		}
	}
	
}
