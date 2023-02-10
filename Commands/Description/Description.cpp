//
// Created by alexandru on 12/31/22.
//

#include "Description.h"

Description::Description() : Command("description"){

}


bool Description::Execute(std::shared_ptr<Networking> myObj) {
    MySQL mySql(dataBasePath);
    std::string isbn  = myObj->receiveMessage();
    std::string description =mySql.GetDataFromQuery("SELECT * FROM DESCRIPTIONS WHERE ISBN='"+isbn+"'").at(0).at(0);
    std::cerr<<description<<"\n";
    myObj->sendMessage(description);
    return true;
}
