#ifndef SERVER_H
#define SERVER_H

#include <iostream>

#include "asio.hpp"
#include "BoiteAuxLettres.h"

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

//namespace centralesupelec::tp2
//{
class Request;

namespace ip = asio::ip;
namespace chrono = std::chrono;

class Server
{
public:
	enum class State
	{
		STOPPED,
		WAITING_FOR_CONNECTION,
		WAITING_FOR_PASSWORD,
		TALKING_TO_USER,
		QUITTING
	};

	explicit Server(asio::io_context& io, BoiteAuxLettres boite) ;

private:
	// Beaucoup de protocoles sont spécifiés sous la forme d'une
	// machine à états, il est donc naturel de les implémenter de la
	// même manière (cela facilite le travail de vérification de la
	// correction par rapport aux spécifications).
	State m_currentState{State::STOPPED};

	// La référence au contexte ASIO, nécessaire pour orchestrer les
	// communications réseau
	asio::io_context& m_io;
	// L'objet qui permet d'écouter sur le port réseau et de créer une
	// socket pour communiquer avec un client qui se présente
	ip::tcp::acceptor m_acceptor;
	// On utilise directement un object iostream qui représente le canal de
	// communication fourni par la connexion TCP/IP.
	// On peut lire et écrire sur ce canal avec les opérateurs habituels <<
	// et >>.
	ip::tcp::iostream m_stream;

	//On associe une boite aux lettres au serveur (en réalité on devrait pouvoir en gérer plusieurs)
	BoiteAuxLettres boiteAuxLettres;

	// Le port TCP/IP sur lequel le programme communique.
	constexpr static int ECHO_PORT = 9999;

	//void process(const std::string& req);

public:
	void start();

	void handleRequest(RequestUser& req);
	void handleRequest(RequestPass& req);
	void handleRequest(Request& req);
	void handleRequest(RequestQuit& req);
	void handleRequest(RequestDele& req);
	void handleRequest(RequestStat& req);
	void handleRequest(RequestList& req);
	void handleRequest(RequestRetr& req);
	void handleRequest(RequestNoop& req);
	void handleRequest(RequestRst& req);
	void handleRequest(RequestTop& req);
	void handleRequest(RequestUidl& req);
};


#endif
