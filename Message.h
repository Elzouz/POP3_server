#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <string>
#include <ostream>
#include <chrono>

class Message
{
private:
    int id;
    std::string uidl{""};
    void setUidl();
    static int counter_;
    std::string subject;
    std::string expeditor;
    std::chrono::system_clock::time_point date;
    std::string content;
    int size; // taille du message en octets
    bool toBeDeleted{false};
    void setSize(int s);

public:
    Message(const std::string& subject, const std::string& expeditor, const std::chrono::system_clock::time_point& date, const std::string& content);

    int getId() const;
    const std::string& getUidl() const;
    const std::string& getSubject() const;
    const std::string& getExpeditor() const;
    const std::chrono::system_clock::time_point& getDate() const;
    const std::string& getContent() const;
    bool isToBeDeleted() const;
    void markToBeDeleted();
    bool doNotDelete();
    int getSize() const;
    std::string msgToSize(); // Construit une partie du message et fixe sa taille

        
    std::string affichageHorodatage(const std::chrono::system_clock::time_point& horodatage);
};

std::ostream& operator<<(std::ostream& os, Message& msg);

#endif