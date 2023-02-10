//
// Created by alexandru on 12/29/22.
//

#ifndef BLIPBUG_RATE_H
#define BLIPBUG_RATE_H
#include "../AbstractClass/Command.h"

class Rate: public Command{

public:
    Rate();
    bool Execute(std::shared_ptr<Networking> myObj) override;

};


#endif //BLIPBUG_RATE_H
