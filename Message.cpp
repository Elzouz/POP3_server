#include "Message.h"
#include <iostream>
#include <chrono>
#include <random>
#include <iomanip>

Message::Message(const std::string& s, const std::string& exp, const std::chrono::system_clock::time_point& d, const std::string& cont) : 
    id{counter_++},    
    subject{s}, 
    expeditor{exp},
    date{d},
    content{cont}
    {
        msgToSize(); // Initialise la taille du message lors de sa création
        setUidl();  // Initialise l'identifiant unique du message
    }

int Message::counter_ = 1;

int Message::getId() const{
    return id;
}

void Message::setUidl() {
    static std::random_device rd;
    static std::mt19937 gen(rd());  // générateur de nombres aléatoires
    static std::uniform_int_distribution<int> random_chars(0x21, 0x7E); // caractères ascii dstribués uniformément
    std::uniform_int_distribution<int> random_length(1, 70);
    int length = random_length(gen);
    std::string result;
    for (int i = 0; i < length; ++i) {
        result.push_back(static_cast<char>(random_chars(gen)));
    }
    uidl = result;
}

const std::string& Message::getUidl() const {
    return uidl;
}

const std::string& Message::getSubject() const{
    return subject;
}

const std::string& Message::getExpeditor() const{
    return expeditor;
}

const std::chrono::system_clock::time_point& Message::getDate() const{
    return date;
}

const std::string& Message::getContent() const{
    return content;
}

bool Message::isToBeDeleted() const{
    return toBeDeleted;
}

void Message::markToBeDeleted(){
    toBeDeleted = true;
}

bool Message::doNotDelete() {
    toBeDeleted = false;
    return toBeDeleted;
}

int Message::getSize() const{
    return size;
}

void Message::setSize(int s){
    size = s;
}

std::string Message::msgToSize(){
    std::string sizetocount = "Id: " + std::to_string(id) + "\r\n"
       + "Subject: " + subject + "\r\n"
       + "From: " + expeditor + "\r\n"
       + "Date: " + affichageHorodatage(date) + "\r\n"
       + "Content: " + content + "\r\n";
    setSize(sizetocount.size());
    return sizetocount; // Retourne la partie du message utilisée pour calculer sa taille
}

std::string Message::affichageHorodatage(const std::chrono::system_clock::time_point& horodatage) {
    std::time_t t = std::chrono::system_clock::to_time_t(horodatage);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&t), "%F %T");
    return oss.str();
}

std::ostream& operator<<(std::ostream& os, Message& msg)
{
    auto h = msg.getDate();
    std::string sizetocount = msg.msgToSize();
    os << sizetocount
       << "To be deleted: " << (msg.isToBeDeleted() ? "Yes" : "No") << "\r\n"
       << ".";
    return os;
}