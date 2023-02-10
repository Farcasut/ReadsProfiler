//
// Created by alexandru on 12/11/22.
//

#ifndef BLIPBUG_REGISTER_H
#define BLIPBUG_REGISTER_H
#include "../AbstractClass/Command.h"

class Register: public Command{
public:
    Register();
    bool Execute(std::shared_ptr<Networking> myObj) override ;
};


#endif //BLIPBUG_REGISTER_H
