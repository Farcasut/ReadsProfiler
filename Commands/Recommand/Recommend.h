//
// Created by alexandru on 12/29/22.
//

#ifndef BLIPBUG_RECOMMEND_H
#define BLIPBUG_RECOMMEND_H
#include "../AbstractClass/Command.h"



class Recommend: public Command {
public:
    Recommend();
    bool Execute(std::shared_ptr<Networking> myObj) override;
    void sendBooks(const std::vector<std::vector<std::string>> &books, std::shared_ptr<Networking> myObj);
};


#endif //BLIPBUG_RECOMMEND_H
