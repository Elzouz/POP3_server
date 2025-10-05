#ifndef SERVER_H
#define SERVER_H

#include <iostream>

#include "asio.hpp"

class Request;

namespace ip = asio::ip;
namespace chrono = std::chrono;

class Server
{
private:
	// Beaucoup de protocoles sont spécifiés sous la forme d'une
	// machine à états, il est donc naturel de les implémenter de la
	// même manière (cela facilite le travail de vérification de la
	// correction par rapport aux spécifications).
	enum class State
	{
		STOPPED,
		WAITING_FOR_CONNECTION,
		TALKING_TO_USER,
		QUITTING
	};
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

	// Le port TCP/IP sur lequel le programme communique.
	constexpr static int ECHO_PORT = 9999;

	std::string process(const std::string& req);

public:
	explicit Server(asio::io_context& io);

	void start();
};

#endif // !defined(SERVER_H)
