//
// Created by alexandru on 12/11/22.
//

#include "Login.h"
Login::Login() : Command("login") {
    this->username="";
}

bool Login::Execute(std::shared_ptr<Networking> myObj)
{
    std::string username=myObj->receiveMessage();
    std::string password=myObj->receiveMessage();
    MySQL mysql(dataBasePath);

    if(mysql.CheckIfUserExists(username, password))
    {
        myObj->sendMessage("succes");
        this->username=username;
        return true;
    }

    myObj->sendMessage("failure");
    return false;
}

std::string Login::GetUsername() const {
    return this->username;
}
