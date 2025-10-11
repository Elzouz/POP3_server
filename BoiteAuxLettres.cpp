#include "BoiteAuxLettres.h"
#include "Message.h"
#include <algorithm>

void BoiteAuxLettres::AddMessage(const Message& msg){
    listeMessages.push_back(msg);
}

void BoiteAuxLettres::deleteMessage(){
    listeMessages.erase(std::remove_if(listeMessages.begin(), listeMessages.end(),
        [](const Message& msg) { return msg.isToBeDeleted(); }),
        listeMessages.end());
}

Message* BoiteAuxLettres::getMessageById(int id) {
    auto it = std::find_if(listeMessages.begin(), listeMessages.end(),
                           [id](const Message& m){ return m.getId() == id; });
    return (it != listeMessages.end()) ? &*it : nullptr;
}

std::vector<Message>& BoiteAuxLettres::getAllMessages(){
    return listeMessages;
}

std::pair<int, int> BoiteAuxLettres::countSize(){
    int totalSize = 0;
    int messageCount = 0;
    for (Message& msg : listeMessages) {
        if(!msg.isToBeDeleted()) {
            totalSize += msg.getSize();
            messageCount++;
        }
    }
    return std::make_pair(messageCount, totalSize);
}