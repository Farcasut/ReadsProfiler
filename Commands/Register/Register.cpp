//
// Created by alexandru on 12/11/22.
//

#include "Register.h"

Register::Register(): Command("register"){

}

bool Register::Execute(std::shared_ptr<Networking> myObj) {

    //TODO send to the client if the user is valid
    std::string username =  myObj->receiveMessage();
    std::string password = myObj->receiveMessage();
    MySQL mysql(dataBasePath);
    if(mysql.InsertUserIntoTable(username, password))
    {
        myObj->sendMessage("success");
        return true;
    }
    myObj->sendMessage("failure");
    return false;
}
