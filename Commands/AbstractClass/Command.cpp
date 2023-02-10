//
// Created by alexandru on 12/11/22.
//


#include "Command.h"
Command::Command(std::string n) :name(n), username(""){}

void Command::SetUsername(const std::string &username) {
this->username=username;
}
std::string Command::GetName() const {return this->name;}
