//
// Created by alexandru on 12/16/22.
//

#ifndef BLIPBUG_DOWNLOAD_H
#define BLIPBUG_DOWNLOAD_H
#include "../AbstractClass/Command.h"


class Download : public Command{
private:
    bool logged;
public:
    Download();
    bool Execute(std::shared_ptr<Networking> myObj) override;
};


#endif //BLIPBUG_DOWNLOAD_H
