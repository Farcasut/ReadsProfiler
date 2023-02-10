//
// Created by alexandru on 12/31/22.
//

#ifndef BLIPBUG_DESCRIPTION_H
#define BLIPBUG_DESCRIPTION_H
#include "../AbstractClass/Command.h"

class Description: public Command{
public:
    Description();
    bool Execute(std::shared_ptr<Networking> myObj) override;
};


#endif //BLIPBUG_DESCRIPTION_H
