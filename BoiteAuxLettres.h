#ifndef BOITE_AUX_LETTRES_H
#define BOITE_AUX_LETTRES_H

#include <vector>
#include "Message.h"
class BoiteAuxLettres
{
private:
    std::vector<Message> listeMessages;

public:
    void AddMessage(const Message& msg);
    void deleteMessage();
    Message* getMessageById(int id);
    std::vector<Message>& getAllMessages();
    std::pair<int, int> countSize();
};

#endif
