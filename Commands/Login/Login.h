//
// Created by alexandru on 12/11/22.
//

#ifndef BLIPBUG_LOGIN_H
#define BLIPBUG_LOGIN_H
#include "../AbstractClass/Command.h"
class Login : public Command{
    std::string username;
public:
    Login();
    bool Execute(std::shared_ptr<Networking> myObj) override ;
    std::string GetUsername() const;

};


#endif //BLIPBUG_LOGIN_H
