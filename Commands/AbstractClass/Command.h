//
// Created by alexandru on 12/11/22.
//

#ifndef BLIPBUG_COMMAND_H
#define BLIPBUG_COMMAND_H

#include <string>
#include <memory>
#include "../../MySQL/MySQL.h"
#include "../../Networking/Networking.h"

class Command {
protected:
    std::string name;
    std::string username;
public:
    static std::string dataBasePath;
public:
    Command(std::string n);
    virtual std::string GetName() const;
    virtual bool Execute(std::shared_ptr<Networking> myObj)=0;
    void SetUsername(const std::string & username);
};


#endif //BLIPBUG_COMMAND_H
